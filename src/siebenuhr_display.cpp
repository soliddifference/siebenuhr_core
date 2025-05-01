#include <Arduino.h>

#include "siebenuhr_core.h"
#include "siebenuhr_display.h"
#include "siebenuhr_eeprom.h"

#include "FX/snake.h"
#include "Personalities/FixedColorRenderer.h"
#include "Personalities/ColorWheelRenderer.h"
#include "Personalities/RainbowRenderer.h"
#include "Personalities/MosaikRenderer.h"

namespace siebenuhr_core
{
    Display* Display::s_instance = nullptr;

    Display* Display::getInstance() 
    {
        if (Display::s_instance == nullptr) {
            Display::s_instance = new Display();
        }
        return Display::s_instance;
    }

    Display::Display()
        : m_avgComputionTime(100)
        , m_renderer(nullptr)
    {
    }

    void Display::initialize(ClockType clockType, int numGlyphs)
    {
        m_clockType = clockType;
        m_numGlyphs = numGlyphs;
        if (m_clockType == ClockType::CLOCK_TYPE_MINI) {
            // Setup mini clock
            initializeGlyphs(constants::SegmentCount, constants::MiniLedsPerSegment);
        } else {
            // Setup regular clock
            initializeGlyphs(constants::SegmentCount, constants::RegularLedsPerSegment);
        }
        m_powerEnabled = true;

        // set saved configuration
        m_nBrightness = Configuration::loadBrightness();
        setBrightness(m_nBrightness);

        setText("7uhr");
    }

    void Display::setHeartbeatEnabled(bool isEnabled) 
    {
        m_heartbeatEnabled = isEnabled;
        if (m_heartbeatEnabled)
        {
            pinMode(constants::LED1_PIN, OUTPUT);
        }
    }

    void Display::initializeGlyphs(int numSegments, int ledsPerSegment) 
    {
        m_numSegments = numSegments;
        m_numLEDsPerSegments = ledsPerSegment;

        assert(m_numGlyphs > 0 && "Glyph count must be greater 0.");
        assert(m_numSegments > 0 && "Segnent count per glyph must be greater 0.");
        assert(m_numLEDsPerSegments > 0 && "LED count per segment must be greater 0.");

        m_numLEDs = m_numSegments * m_numLEDsPerSegments * m_numGlyphs;
        m_LEDs = new CRGB[m_numLEDs];

        m_glyphs = new Glyph*[m_numGlyphs];
        for (size_t i = 0; i < m_numGlyphs; ++i) 
        {
            m_glyphs[i] = new Glyph(m_numSegments, m_numLEDsPerSegments);
            m_glyphs[i]->attach(i, m_numGlyphs, m_LEDs);
            // todo: redo everything here! -> proper initialization from glyph to effect..
            // m_glyphs[i]->setEffect(new SnakeFX(m_numLEDs, m_numLEDsPerSegments));
        }

		FastLED.addLeds<WS2812, constants::GLYPH_LED_PIN, GRB>(m_LEDs, m_numLEDs);      
        FastLED.clear(true);

        m_lastUpdateMillis = millis();

        logMessage(LOG_LEVEL_INFO, "Display Configuration: Glyphs=%d, Segments=%d, LEDs per Segment=%d, Total LEDs=%d", 
            m_numGlyphs, m_numSegments, m_numLEDsPerSegments, m_numLEDs);
    }

    void Display::setPowerEnabled(bool isEnabled) 
    {
        m_powerEnabled = isEnabled;
        if (!m_powerEnabled) 
        {
            FastLED.clear(true);
        }
    }
    
    float smoothedLux = 0.0f;  // Keep this as a static/global variable
    constexpr float MIN_LUX = 2.0f;
    constexpr float MAX_LUX = 40.0f;

    uint8_t getSmoothedBrightnessFromLux(float lux, uint8_t maxBrightnessRange) {
        // Clamp lux to your sensor range
        if (lux < MIN_LUX) lux = MIN_LUX;
        if (lux > MAX_LUX) lux = MAX_LUX;

        // Exponential moving average (smoothing factor)
        const float alpha = 0.025f;  // lower = smoother, higher = more responsive
        smoothedLux = (1.0f - alpha) * smoothedLux + alpha * lux;

        // Map smoothed lux to brightness range
        float normalized = (smoothedLux - MIN_LUX) / (MAX_LUX - MIN_LUX);
        if (normalized < 0.0f) normalized = 0.0f;
        if (normalized > 1.0f) normalized = 1.0f;

        uint8_t brightness = (uint8_t)(normalized * (maxBrightnessRange));
        return brightness;
    }

    // void Display::setEnvLightLevel(float currentLux, int minBrightness, int maxBrightness)
    void Display::setEnvLightLevel(float currentLux, int baseBrightness, int maxBrightnessRange)
    {
        if (m_powerEnabled) 
        {
            uint8_t brightness = getSmoothedBrightnessFromLux(currentLux, maxBrightnessRange);

            int new_brightness = clamp(baseBrightness + brightness, 0, 255);

            FastLED.setBrightness(new_brightness);

            // logMessage(LOG_LEVEL_INFO, "Base: %d Smoothed: %d Brightness: %d", baseBrightness, brightness, new_brightness);
        }
    }

    void Display::setBrightness(int value, bool saveToEEPROM) 
    {
        value = clamp(value, 0, 255);

        if (saveToEEPROM) {
            Configuration::saveBrightness(value);
        }

        m_nBrightness = value;
        FastLED.setBrightness(m_nBrightness);

        logMessage(LOG_LEVEL_INFO, "Display Brightness: %d", m_nBrightness);
    }

    int Display::getBrightness() 
    {
        return m_nBrightness;
    }

    bool Display::setRenderer(std::unique_ptr<IDisplayRenderer> renderer)
    {
        logMessage(LOG_LEVEL_INFO, "Changing display renderer...");
        m_renderer = std::move(renderer);
        if (m_renderer)
        {
            logMessage(LOG_LEVEL_INFO, "Initializing new renderer...");
            m_renderer->initialize(m_glyphs, m_numGlyphs);
            m_renderer->setText(m_text);
            logMessage(LOG_LEVEL_INFO, "Renderer initialized successfully");
            
            return true;
        }

        logMessage(LOG_LEVEL_WARN, "Failed to set renderer - renderer is null");
        return false;
    }
    
    void Display::setText(const std::string& text)
    {
        m_text = text;
        if (m_renderer)
            m_renderer->setText(text);
    }   

    void Display::setNotification(const std::string& text, int duration)
    {
        m_notificationText = text;
        m_notificationDuration = duration;
        logMessage(LOG_LEVEL_INFO, "Notification set: %s", text.c_str());
    }

    void Display::setColor(const CRGB& color, int steps)
    {
        m_currentColor = color;
        if (m_renderer->supportsColor()) {
            m_renderer->setColor(color);
        }
    }

    CRGB Display::getColor()
    {
        return m_currentColor;
    }

    void Display::setTime(int hours, int minutes) 
    {
        m_hours = hours;
        m_minutes = minutes;

        char formatted_time[5];  // Buffer for "HHMM" + null terminator
        snprintf(formatted_time, sizeof(formatted_time), "%02d%02d", hours, minutes);
        setText(std::string(formatted_time));

        logMessage(LOG_LEVEL_INFO, "Time set: %02d:%02d", hours, minutes); 
    }    

    std::unique_ptr<IDisplayRenderer> Display::createRenderer(PersonalityType personality, const CRGB& defaultColor)
    {
        logMessage(LOG_LEVEL_INFO, "Creating new renderer for personality type: %d", static_cast<int>(personality));
        
        std::unique_ptr<IDisplayRenderer> renderer;
        switch (personality)
        {
            case PersonalityType::PERSONALITY_SOLIDCOLOR:
                renderer = std::unique_ptr<IDisplayRenderer>(new FixedColorRenderer(defaultColor));
                logMessage(LOG_LEVEL_INFO, "Created SolidColor renderer with default color: R=%d, G=%d, B=%d", 
                    defaultColor.r, defaultColor.g, defaultColor.b);
                break;
            case PersonalityType::PERSONALITY_COLORWHEEL:
                renderer = std::unique_ptr<IDisplayRenderer>(new ColorWheelRenderer(defaultColor));
                logMessage(LOG_LEVEL_INFO, "Created ColorWheel renderer");
                break;
            case PersonalityType::PERSONALITY_RAINBOW:
                renderer = std::unique_ptr<IDisplayRenderer>(new RainbowRenderer());
                logMessage(LOG_LEVEL_INFO, "Created Rainbow renderer");
                break;
            case PersonalityType::PERSONALITY_MOSAIK:
                renderer = std::unique_ptr<IDisplayRenderer>(new MosaikRenderer());
                logMessage(LOG_LEVEL_INFO, "Created Rainbow renderer");
                break;
            default:
                logMessage(LOG_LEVEL_WARN, "Unknown personality type %d, defaulting to solid color", static_cast<int>(personality));
                renderer = std::unique_ptr<IDisplayRenderer>(new FixedColorRenderer(defaultColor));
                break;
        }
        return renderer;
    }

    void Display::setPersonality(PersonalityType personality)
    {
        if (m_renderer)
        {
            if (m_currentPersonality == personality)
            {
                logMessage(LOG_LEVEL_INFO, "Personality already set to %d", static_cast<int>(personality));
                return;
            }

            if (m_renderer->supportsColor())
            {
                m_currentColor = m_renderer->getColor();
                logMessage(LOG_LEVEL_INFO, "Preserving current color: R=%d, G=%d, B=%d", m_currentColor.r, m_currentColor.g, m_currentColor.b);
            }
        }

        // Create and set new renderer
        if (setRenderer(createRenderer(personality, m_currentColor)))
        {
            m_currentPersonality = personality;
        }
    }

    void Display::selectAdjacentPersonality(int direction)
    {
        // Get the current personality index
        int currentIndex = static_cast<int>(m_currentPersonality);
        
        // Calculate new index with wrapping
        int numPersonalities = static_cast<int>(PersonalityType::PERSONALITY_END);
        int newIndex = (currentIndex + direction + numPersonalities) % numPersonalities;
        
        // Set the new personality
        setPersonality(static_cast<PersonalityType>(newIndex));
        
        logMessage(LOG_LEVEL_INFO, "Selected %s personality: %d", direction > 0 ? "next" : "previous", newIndex);
    }

    void Display::update() 
    {
        unsigned long currentMillis = millis();

        if (!m_powerEnabled) 
        {
            // turn clock off
            for (size_t i = 0; i < m_numGlyphs; ++i) 
            {
                m_glyphs[i]->resetLEDS();
            }
            FastLED.show();
        }
        else
        {
            if (m_renderer)
                m_renderer->update(currentMillis, m_hours, m_minutes);

            FastLED.show();
        }

        // update heartbeat led
        if (m_heartbeatEnabled && (currentMillis - m_lastHeartbeatTime >= m_heartbeatInterval)) {
            m_lastHeartbeatTime = currentMillis;
            m_heartbeatState = !m_heartbeatState;
            digitalWrite(constants::GLYPH_LED_PIN, m_heartbeatState);

            // logMessage(LOG_LEVEL_INFO, "ø FrameTime: %f", m_avgComputionTime.getAverage());
        }

        m_avgComputionTime.addValue(currentMillis - m_lastUpdateMillis);
        m_lastUpdateMillis = currentMillis;
    }

}