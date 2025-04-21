#include <Arduino.h>

#include "siebenuhr_core.h"
#include "siebenuhr_display.h"
#include "siebenuhr_eeprom.h"

#include "FX/snake.h"

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
            pinMode(m_heartbeatPin, OUTPUT);
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

		FastLED.addLeds<WS2812, constants::PinLEDs, GRB>(m_LEDs, m_numLEDs);      
        FastLED.clear(true);

        m_lastUpdateMillis = millis();

        logMessage(LOG_LEVEL_INFO, "Display setup: #Glyphs=%d #Seg=%d #LEDpSeg=%d #LEDs=%d", m_numGlyphs, m_numSegments, m_numLEDsPerSegments, m_numLEDs);
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

    uint8_t getSmoothedBrightnessFromLux(float lux, uint8_t minBrightness, uint8_t maxBrightness) {
        // Clamp lux to your sensor range
        if (lux < 2.0f) lux = 2.0f;
        if (lux > 40.0f) lux = 40.0f;

        // Exponential moving average (smoothing factor)
        const float alpha = 0.025f;  // lower = smoother, higher = more responsive
        smoothedLux = (1.0f - alpha) * smoothedLux + alpha * lux;

        // Map smoothed lux to brightness range
        float normalized = (smoothedLux - 2.0f) / (40.0f - 2.0f);
        if (normalized < 0.0f) normalized = 0.0f;
        if (normalized > 1.0f) normalized = 1.0f;

        uint8_t brightness = (uint8_t)(minBrightness + normalized * (maxBrightness - minBrightness));
        return brightness;
    }

    void Display::setEnvLightLevel(float currentLux, int minBrightness, int maxBrightness)
    {
        uint8_t brightness = getSmoothedBrightnessFromLux(currentLux, minBrightness, maxBrightness);
        FastLED.setBrightness(brightness);

        // logMessage(LOG_LEVEL_INFO, "Brightness brightness:%d lux:%f", brightness, currentLux);
    }

    void Display::setBrightness(int value, bool saveToEEPROM) {
        value = clamp(value, 0, 255);

        if (saveToEEPROM) {
            Configuration::saveBrightness(value);
        }

        m_nBrightness = value;
        FastLED.setBrightness(m_nBrightness);

        logMessage(LOG_LEVEL_INFO, "Display Brightness: %d", m_nBrightness);
    }

    void Display::setText(const std::string& text)
    {
        m_text = text;

        if (m_glyphs && m_numGlyphs > 0) 
        {
            size_t text_length = m_text.length();
            for (size_t i = 0; i < m_numGlyphs; ++i) 
            {
                if (i < text_length)
                {
                    m_glyphs[i]->setAscii(m_text[i]);
                }
                else
                {
                    m_glyphs[i]->setAscii(' ');
                }
            }                        
        }
    }

    void Display::setColor(const CRGB& color, int steps)
    {
        for (size_t i = 0; i < m_numGlyphs; ++i) 
        {
            m_glyphs[i]->setColor(color);
        }
    }

    void Display::update() 
    {
        unsigned long currentMillis = millis();

        // update glyphs
        if (m_powerEnabled && (currentMillis - m_lastUpdateTime >= 1000 / constants::FPS)) // Adjust for FPS
        {
            m_lastUpdateTime = currentMillis;

            for (size_t i = 0; i < m_numGlyphs; ++i) 
            {
                m_glyphs[i]->update(currentMillis);
            }

            FastLED.show();
        }

        // update heartbeat led
        if (m_heartbeatEnabled && (currentMillis - m_lastHeartbeatTime >= m_heartbeatInterval)) {
            m_lastHeartbeatTime = currentMillis;
            m_heartbeatState = !m_heartbeatState;
            digitalWrite(m_heartbeatPin, m_heartbeatState);

            logMessage(LOG_LEVEL_INFO, "ø FrameTime: %f", m_avgComputionTime.getAverage());
        }

        m_avgComputionTime.addValue(currentMillis - m_lastUpdateMillis);
        m_lastUpdateMillis = currentMillis;
    }

}