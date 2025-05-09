#include "siebenuhr_controller.h"
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_INA219.h>

BH1750 g_bh1750; // Default address 0x23
Adafruit_INA219 g_ina219; // Default address 0x40

namespace siebenuhr_core {

const BaseController::ControllerMenu_t BaseController::m_menu[BaseController::m_menuMaxEntries] = {
    {CONTROLLER_MENU::BRIGHTNESS, "Brightness", "Brit"},
    {CONTROLLER_MENU::HUE, "Hue", "COLr"},
};

void BaseController::initialize(ClockType type)
{
    Logger::init("🚀 siebenuhr.core");

    m_clockType = type;

    m_display = Display::getInstance();
    if (m_display != nullptr)
    {
        m_display->initialize(type, 4);
        m_display->setHeartbeatEnabled(false);

        Wire.begin(constants::SDA_PIN, constants::SCL_PIN);
        if (g_bh1750.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) 
        {
            LOG_I("BH1750 Initialized");
            m_isBH1750Initialized = true;
        } 
        else 
        {
            LOG_E("Error initialising BH1750");
        }

        if (g_ina219.begin()) 
        {
            LOG_I("INA219 Initialized");
            m_isINA219Initialized = true;
        } else 
        {
            LOG_E("Error initialising INA219");
        }
    }

    m_menuPosLastTimeChange = millis();

    // setup hardware controls
    initializeControls();
}

void BaseController::initializeControls()
{
    m_encoder = new UIKnob(constants::ROT_ENC_A_PIN, constants::ROT_ENC_B_PIN, constants::ROT_ENC_BUTTON_PIN);
    m_encoder->setEncoderBoundaries(1, 255, 128, false);

    m_button1 = new UIButton(constants::USER_BUTTON_PIN, constants::LED4_PIN);
    m_button2 = new UIButton(constants::BOOT_BUTTON_PIN, constants::LED3_PIN);
}

Display* BaseController::getDisplay()
{
    if (m_display == nullptr)
    {
        LOG_I("Display not set. Initialize not called (yet) or something went wrong in the setup!?");
        m_display = Display::getInstance();
    }
    return m_display;
}

int BaseController::calculateHue(const CRGB& color)
{
    // Convert RGB to HSV hue value (0-255)
    if (color.r == color.g && color.g == color.b) {
        return 0; // Grayscale has no hue
    }
    
    int max = color.r;
    if (color.g > max) max = color.g;
    if (color.b > max) max = color.b;
    
    int min = color.r;
    if (color.g < min) min = color.g; 
    if (color.b < min) min = color.b;

    int hue = 0;
    if (max == color.r) {
        hue = 43 * (color.g - color.b) / (max - min);
    } else if (max == color.g) {
        hue = 85 + 43 * (color.b - color.r) / (max - min);
    } else {
        hue = 171 + 43 * (color.r - color.g) / (max - min);
    }

    if (hue < 0) hue += 256;
    return hue;
}

void BaseController::setMenu(CONTROLLER_MENU menu) {
    m_menuCurPos = menu;
    m_menuPosLastTimeChange = millis();
   
    switch (m_menuCurPos) {			
    case CONTROLLER_MENU::BRIGHTNESS:
    {
        int current_brightness = getDisplay()->getBrightness();
        m_encoder->setEncoderBoundaries(1, 255, current_brightness);
        LOG_I("Switch to Option: %s, value=%d", m_menu[static_cast<int>(m_menuCurPos)].name.c_str(), current_brightness);
        break;
    }
    case CONTROLLER_MENU::HUE: 
    {
        CRGB current_color = getDisplay()->getColor();
        int current_hue = calculateHue(current_color);

        m_encoder->setEncoderBoundaries(0, 255, current_hue, true);
        LOG_I("Switch to Option: %s, hue=%d", m_menu[static_cast<int>(m_menuCurPos)].name.c_str(), current_hue);
        break;
    }
    }
}

void BaseController::handleMenuChange() 
{
    // handle encoder changes
    if (m_encoder != nullptr)
    {
        m_encoder->update();

        if(m_encoder->isButtonReleased()) 
        {
            switch (m_menuCurPos) {			
            case CONTROLLER_MENU::BRIGHTNESS: {
                    getDisplay()->setNotification(" huE", 1500);
                    setMenu(CONTROLLER_MENU::HUE);
                    break;
                }
            case CONTROLLER_MENU::HUE: {
                    getDisplay()->setNotification("brit", 1500);
                    setMenu(CONTROLLER_MENU::BRIGHTNESS);
                    break;
                }
            }
        }

        // handle manual value changes of the appropriate menu
        if (m_encoder->hasPositionChanged()) 
        {          
            switch (m_menuCurPos) {
            case CONTROLLER_MENU::BRIGHTNESS: {
                handleManualBrightnessChange();
                break;
            }
            case CONTROLLER_MENU::HUE: {
                handleManualHueChange();
                break;
            }
            }
            m_menuPosLastTimeChange = millis(); // reset timeout
        }
    }

    if (m_button1 && m_button2)
    {
        auto button1_state = m_button1->update();
        auto button2_state = m_button2->update();    

        if (m_clockType == ClockType::CLOCK_TYPE_REGULAR)
        {                
            if (button1_state == ButtonState::SingleClick) {
                getDisplay()->selectAdjacentPersonality(-1);
            }
            if (button2_state == ButtonState::SingleClick) {
                getDisplay()->selectAdjacentPersonality(1);
            }
        }
        else if (m_clockType == ClockType::CLOCK_TYPE_MINI)
        {
            // single click and long press actions for brightness and hue config
            if (button1_state == ButtonState::SingleClick || button2_state == ButtonState::SingleClick) 
            {
                switch (m_menuCurPos) {
                case CONTROLLER_MENU::BRIGHTNESS: {
                        getDisplay()->setNotification(" huE", 1500);
                        setMenu(CONTROLLER_MENU::HUE);
                        break;
                    }
                case CONTROLLER_MENU::HUE: {
                        getDisplay()->setNotification("brit", 1500);
                        setMenu(CONTROLLER_MENU::BRIGHTNESS);
                        break;
                    }
                }
            } 
            // handle long press for brightness change
            else if (m_button1->isLongPress() || m_button2->isLongPress())
            {
                switch (m_menuCurPos) {
                case CONTROLLER_MENU::BRIGHTNESS: {
                    handleManualBrightnessChange();
                    break;
                }
                case CONTROLLER_MENU::HUE: {
                    handleManualHueChange();
                    break;
                }
                }
            }
            else if (button1_state == ButtonState::LongClick || button2_state == ButtonState::LongClick) 
            {
                if ( m_menuCurPos == CONTROLLER_MENU::HUE) 
                {
                    CRGB color = CHSV(m_currentHue, 255, 255);
                    if (!sendColorToHomeAssistant(color))
                    {
                        getDisplay()->setColor(color);
                    }
                    m_currentHue = -1;
                }
            }                  
            // change personality on double click
            else if (button1_state == ButtonState::DoubleClick) 
            {
                getDisplay()->selectAdjacentPersonality(-1);
            }
            else if (button2_state == ButtonState::DoubleClick) 
            {
                getDisplay()->selectAdjacentPersonality(1);
            }
        }
    }
}

void BaseController::handleManualBrightnessChange()
{
    long brightness = m_currentBrightness; 

    if (m_clockType == ClockType::CLOCK_TYPE_REGULAR)
    {
        brightness = m_encoder->getPosition(); 
    }
    else
    {
        if (m_button1->isLongPress())
        {
            brightness -= 1;
        }
        else if (m_button2->isLongPress())
        {
            brightness += 1;
        }
        brightness = clamp(brightness, 1L, 255L);
    }

    if (m_autoBrightnessEnabled)
    {
        // send state change back to home assistant server
        if (!sendBrightnessToHomeAssistant(brightness))
        {
            m_currentBrightness = brightness;
        }
    }
    else
    {
        if (brightness != getDisplay()->getBrightness())
        {
            // send state change back to home assistant server
            if (!sendBrightnessToHomeAssistant(brightness))
            {
                getDisplay()->setBrightness(brightness);
            }
        }
    }
}

void BaseController::handleManualHueChange()
{
    CRGB color = getDisplay()->getColor();
    if (m_clockType == ClockType::CLOCK_TYPE_REGULAR)
    {
        int hue = m_encoder->getPosition() % 255;
        color = CHSV(hue, 255, 255);
    }
    else
    {
        // using float here for higher resolution
        if (m_currentHue == -1) 
        {
            m_currentHue = calculateHue(getDisplay()->getColor());
        }

        if (m_button1->isLongPress())
        {
            m_currentHue -= .1;
            if (m_currentHue < 0)
            {
                m_currentHue += 255.0f;
            }
        }
        else if (m_button2->isLongPress())
        {
            m_currentHue += .1;
            if (m_currentHue > 255.0f)
            {
                m_currentHue -= 255.0f;
            }
        }
        color = CHSV(((int)m_currentHue) % 255, 255, 255);
        LOG_I("HUE CHANGE: %f", m_currentHue);
    }

    if (m_clockType == ClockType::CLOCK_TYPE_REGULAR)
    {
        if (!sendColorToHomeAssistant(color))
        {
            getDisplay()->setColor(color);
        }
    }
    else
    {
        getDisplay()->setColor(color);
    }
}

void BaseController::update()
{
    handleMenuChange();

    if (m_autoBrightnessEnabled && m_isBH1750Initialized) 
    {
        getDisplay()->setEnvLightLevel(g_bh1750.readLightLevel(), m_currentBrightness, 100);
    }

    if (m_powerMonitoringEnabled && m_isINA219Initialized)
    {
        readAndPrintPowerMonitoring();
    }

    getDisplay()->update();
}

void BaseController::setPower(bool powerEnabled)
{
    getDisplay()->setPowerEnabled(powerEnabled);
    LOG_I("Power set to %s", powerEnabled ? "ON" : "OFF");
}

void BaseController::setBrightness(int value)
{
    if (m_autoBrightnessEnabled)
    {
        m_currentBrightness = value;
        
        if (m_menuCurPos == CONTROLLER_MENU::BRIGHTNESS)
        {
            m_encoder->setPosition(m_currentBrightness);
        }

        LOG_I("Base-Brightness set to %d", m_currentBrightness);
    }
    else
    {
        if (value != m_currentBrightness)
        {
            getDisplay()->setBrightness(value);
            m_currentBrightness = getDisplay()->getBrightness();

            if (m_menuCurPos == CONTROLLER_MENU::BRIGHTNESS)
            {
                m_encoder->setPosition(m_currentBrightness);
            }

            LOG_I("Brightness set to %d", value);
        }
    }
}

void BaseController::setColor(int r, int g, int b)
{
    CRGB color = CRGB(r, g, b);
    getDisplay()->setColor(color);
}

void BaseController::setText(const std::string& text)
{
    getDisplay()->setText(text);
    LOG_I("SET: text=%s", text.c_str());
}

void BaseController::setNotification(const std::string& text, int duration)
{
    getDisplay()->setNotification(text, duration);
    LOG_I("Notification set: %s duration=%d", text.c_str(), duration);
}

void BaseController::setTime(int hours, int minutes) 
{
    m_hours = hours;
    m_minutes = minutes;
    getDisplay()->setTime(hours, minutes);
    LOG_I("Time set: %02d:%02d", hours, minutes);
}

void BaseController::setAutoBrightnessEnabled(bool isEnabled)
{   
    m_autoBrightnessEnabled = isEnabled;
    LOG_I("SET: auto_brightness=%s", m_autoBrightnessEnabled ? "TRUE" : "FALSE");
}

void BaseController::setPowerMonitoringEnabled(bool isEnabled)
{
    m_powerMonitoringEnabled = isEnabled;
}

void BaseController::setPersonality(PersonalityType personality)
{
    if (m_display != nullptr)
    {
        m_display->setPersonality(personality);
    }
}

void BaseController::readAndPrintPowerMonitoring()
{
    if (millis() - m_lastSensorReadTime >= constants::SensorReadInterval) {
        float busvoltage = g_ina219.getBusVoltage_V();
        float shuntvoltage = g_ina219.getShuntVoltage_mV();  // in mV
        float current_mA = shuntvoltage * 100.0;           // for 10 mΩ shunt
        float loadvoltage = busvoltage + (shuntvoltage / 1000.0); // in V
        float power_mW = busvoltage * current_mA;          // V * mA = mW

        bool current_in_A = abs(current_mA) >= 500.0;
        bool power_in_W = abs(power_mW) >= 500.0;

        LOG_I("Bus: %.2fV | Shunt: %.2fmV | Load: %.2fV | Current: %.2f%s | Power: %.2f%s", 
            busvoltage, 
            shuntvoltage, 
            loadvoltage, 
            current_in_A?(current_mA / 1000.0):current_mA, 
            current_in_A?"A":"mA",
            power_in_W?(power_mW / 1000.0):power_mW,
            power_in_W?"W":"mW"
        );
        
        m_lastSensorReadTime = millis();
    }
}

} // namespace siebenuhr_core 