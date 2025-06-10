#pragma once

#include <FastLED.h>
#include "siebenuhr_core.h"
#include "siebenuhr_color.h"
#include "siebenuhr_display.h"
#include "siebenuhr_encoder.h"
#include "siebenuhr_button.h"

namespace siebenuhr_core {

class BaseController {
public:
    virtual ~BaseController() = default;

    // Core functionality
    virtual void initialize(ClockType type);
    virtual void update();

    void setLogLevel(int level) {
        Logger::setLogLevel(static_cast<CoreLogLevel>(level));
    }
    
    // Display control
    Display* getDisplay();
    void setPower(bool powerEnabled);
    void setBrightness(int value);
    void setColor(float hue);
    void setColor(Color color);
    void setText(const std::string& text);
    void setNotification(const std::string& text, int duration);
    void setTime(int hours, int minutes);
    void setPersonality(PersonalityType personality);

    // Settings
    void setAutoBrightnessEnabled(bool isEnabled);
    void setPowerMonitoringEnabled(bool isEnabled);

protected:
    // Virtual methods to be implemented by derived classes
    virtual bool sendBrightnessToHomeAssistant(int brightness) { return false; };
    virtual bool sendColorToHomeAssistant(CRGB color) { return false; };
    virtual void onButtonLongPress() { };
    virtual void onBrightnessChange(int brightness)  { };
    virtual void onColorChange(CRGB color)  { };
    virtual void onPersonalityChange(PersonalityType personality) { };


    // Internal state
    Display* m_display = nullptr;
    ClockType m_clockType;
    UIKnob* m_encoder = nullptr;
    UIButton* m_button1 = nullptr;
    UIButton* m_button2 = nullptr;

    // Menu state
    enum class CONTROLLER_MENU {
        BRIGHTNESS,
        HUE
    };

    struct ControllerMenu_t {
        CONTROLLER_MENU id;
        std::string name;
        std::string shortName;
    };

    static constexpr int m_menuMaxEntries = 2;
    static const ControllerMenu_t m_menu[m_menuMaxEntries];
    CONTROLLER_MENU m_menuCurPos = CONTROLLER_MENU::BRIGHTNESS;
    unsigned long m_menuPosLastTimeChange = 0;

    // Sensor state
    bool m_isBH1750Initialized = false;
    bool m_isINA219Initialized = false;
    bool m_autoBrightnessEnabled = false;
    bool m_powerMonitoringEnabled = false;
    unsigned long m_lastSensorReadTime = 0;

    // Time state
    int m_hours = 0;
    int m_minutes = 0;

    // Color state
    Color m_currentColor;
    int m_currentBrightness = 128;

private:
    void initializeControls();
    void handleUserInput();
    void setMenu(CONTROLLER_MENU menu);
    void handleManualBrightnessChange();
    void handleManualHueChange();
    void readAndPrintPowerMonitoring();

    unsigned long m_lastSimultaneousClickTime = 0;

    static constexpr unsigned long SIMULTANEOUS_CLICK_THRESHOLD = 300;  // ms
    static constexpr unsigned long RESET_HOLD_DURATION = 5000;  // ms
};

} // namespace siebenuhr_core 