#include <Arduino.h>

#include "siebenuhr_core.h"
#include "siebenuhr_display.h"
#include "siebenuhr_eeprom.h"

// #include "FX/snake.h"

#include "Personalities/FixedColorRenderer.h"
#include "Personalities/ColorWheelRenderer.h"
#include "Personalities/RainbowRenderer.h"
#include "Personalities/MosaikRenderer.h"
#include "Personalities/GlitterRenderer.h"

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
        , m_notificationRenderer(new FixedColorRenderer(m_notificationColor))
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
        setBrightness(Configuration::loadBrightness());

        // Initialize notification renderer
        m_notificationRenderer->initialize(m_glyphs, m_numGlyphs);

        setText("7uhr");
    }

    void Display::setHeartbeatEnabled(bool isEnabled) 
    {
        m_heartbeatEnabled = isEnabled;
        if (m_heartbeatEnabled)
        {
            pinMode(constants::LED_HEARTBEAT_PIN, OUTPUT);
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

        // Allocate LED buffer
        m_LEDs = new CRGB[m_numLEDs];
        memset(m_LEDs, 0, m_numLEDs * sizeof(CRGB));

        // Allocate and initialize animation states
        m_animationStates = new LEDAnimationState[m_numLEDs]; 
        memset(m_animationStates, 0, m_numLEDs * sizeof(LEDAnimationState));

        m_glyphs = new Glyph*[m_numGlyphs];
        for (size_t i = 0; i < m_numGlyphs; ++i) 
        {
            m_glyphs[i] = new Glyph(m_numSegments, m_numLEDsPerSegments);
            m_glyphs[i]->attach(i, m_numGlyphs, m_LEDs, m_animationStates);  // Attach to LED buffer and animation states
        }

		FastLED.addLeds<WS2812, constants::LED_GLYPH_PIN, GRB>(m_LEDs, m_numLEDs);      
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

        m_brightness = value;
        FastLED.setBrightness(m_brightness);

        logMessage(LOG_LEVEL_INFO, "Display Brightness: %d", m_brightness);
    }

    int Display::getBrightness() 
    {
        return m_brightness;
    }

    bool Display::setRenderer(std::unique_ptr<IDisplayRenderer> renderer)
    {
        if (renderer == nullptr) {
            return false;
        }
        
        if (m_renderer)
        {
            m_renderer->deactivate();
        }
        m_renderer = std::move(renderer);
        
        if (m_renderer)
        {
            m_renderer->initialize(m_glyphs, m_numGlyphs);
            
            // activate renderer to set the renderer on each glyph
            m_renderer->activate();
            m_renderer->setText(m_text);

            logMessage(LOG_LEVEL_INFO, "%s renderer initialized successfully", m_renderer->getName());
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
        if (!m_notificationRenderer) {
            logMessage(LOG_LEVEL_WARN, "Notification renderer not initialized");
            return;
        }

        m_notificationRenderer->setText(text);
        m_notificationPreviousBrightness = m_brightness;
        m_notificationDuration = duration;
        m_notificationStartTime = millis();
        m_notificationActive = true;

        m_notificationRenderer->activate();

        // Set notification display state
        setBrightness(m_notificationBrightness, false);

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

    void Display::getTime(int& hours, int& minutes)
    {
        hours = m_hours;
        minutes = m_minutes;
    }

    std::unique_ptr<IDisplayRenderer> Display::createRenderer(PersonalityType personality, const CRGB& defaultColor)
    {
        std::unique_ptr<IDisplayRenderer> renderer;
        switch (personality)
        {
            case PersonalityType::PERSONALITY_SOLIDCOLOR:
                renderer = std::unique_ptr<IDisplayRenderer>(new FixedColorRenderer(defaultColor));
                break;
            case PersonalityType::PERSONALITY_COLORWHEEL:
                renderer = std::unique_ptr<IDisplayRenderer>(new ColorWheelRenderer(defaultColor));
                break;
            case PersonalityType::PERSONALITY_RAINBOW:
                renderer = std::unique_ptr<IDisplayRenderer>(new RainbowRenderer());
                break;
            case PersonalityType::PERSONALITY_MOSAIK:
                renderer = std::unique_ptr<IDisplayRenderer>(new MosaikRenderer());
                break;
            case PersonalityType::PERSONALITY_GLITTER:
                renderer = std::unique_ptr<IDisplayRenderer>(new GlitterRenderer(defaultColor));
                break;
            default:
                logMessage(LOG_LEVEL_WARN, "Unknown personality type %d.", static_cast<int>(personality));
                return nullptr;
        }

        logMessage(LOG_LEVEL_INFO, "Created %s renderer for personality type: %d", renderer->getName(), static_cast<int>(personality));

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

    void Display::startLEDAnimation(int ledIndex, const CRGB& targetColor, unsigned long duration)
    {
        if (ledIndex < 0 || ledIndex >= m_numLEDs) return;

        LEDAnimationState& state = m_animationStates[ledIndex];
        state.startColor = m_LEDs[ledIndex];
        state.targetColor = targetColor;
        state.startTime = millis();
        state.duration = duration > 0 ? duration : m_defaultAnimationTime;
        state.isActive = true;
    }

    inline CRGB fadeColor(const CRGB& colorA, const CRGB& colorB, uint8_t t) {
        return CRGB(
            lerp8by8(colorA.r, colorB.r, t),
            lerp8by8(colorA.g, colorB.g, t),
            lerp8by8(colorA.b, colorB.b, t)
        );
    }

    void Display::updateLEDAnimations()
    {
        unsigned long currentMillis = millis();
        for (int i = 0; i < m_numLEDs; ++i)
        {
            LEDAnimationState& state = m_animationStates[i];
            if (!state.isActive) continue;

            // Calculate progress using fixed-point arithmetic (8.8 format)
            // This gives us 256 steps of precision (0-255) for the blend factor
            uint16_t elapsed = currentMillis - state.startTime;
            if (elapsed >= state.duration)
            {
                // Animation complete
                m_LEDs[i] = state.targetColor;
                state.isActive = false;
            }
            else
            {
                // Shift left by 8 bits (equivalent to * 256) to get 8.8 fixed point format
                // TODO: need to check if this works the lower 8 bits contain our blend factor (0-255) and a typecast to uint8_t is not needed
                uint16_t blendFactor = (elapsed << 8) / state.duration;
                
                // Blend between start and target colors using the lower 8 bits directly
                m_LEDs[i] = fadeColor(state.startColor, state.targetColor, blendFactor);
            }
        }
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
            // Handle notification timing and display
            if (m_notificationActive) {
                if (currentMillis - m_notificationStartTime >= m_notificationDuration) {
                    // Notification ended, restore state and brightness
                    setBrightness(m_notificationPreviousBrightness, false);
                    m_notificationActive = false;

                    // activate renderer to set the renderer on each glyph
                    m_renderer->activate();
                    m_renderer->setText(m_text);

                    logMessage(LOG_LEVEL_INFO, "Notification ended, restored normal display");
                } else {
                    // Update notification display
                    m_notificationRenderer->update();
                }
            } else if (m_renderer) {
                // Update normal display
                m_renderer->update();
            }

            updateLEDAnimations();
            FastLED.show();
        }

        // update heartbeat led
        if (m_heartbeatEnabled && (currentMillis - m_lastHeartbeatTime >= m_heartbeatInterval)) {
            m_lastHeartbeatTime = currentMillis;
            m_heartbeatState = !m_heartbeatState;
            digitalWrite(constants::LED_HEARTBEAT_PIN, m_heartbeatState);
        }

        m_avgComputionTime.addValue(currentMillis - m_lastUpdateMillis);
        m_lastUpdateMillis = currentMillis;
    }

}