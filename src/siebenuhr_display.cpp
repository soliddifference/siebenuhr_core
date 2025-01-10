#include <Arduino.h>

#include "siebenuhr_core.h"
#include "siebenuhr_display.h"

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

    void Display::initialize(ClockType clockType, int numGlyphs)
    {
        Serial.begin(115200);

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
        m_nBrightness = constants::DefaultBrightness;
       	// TODO: _nBrightness = _inst->readFromEEPROM(EEPROM_ADDRESS_BRIGHTNESS);
        setBrightness(m_nBrightness);
    }

    // Display::Display(int numGlyphs, int numSegments, int ledsPerSegment)
    //     : m_numGlyphs(numGlyphs)
    //     , m_numSegments(numSegments)
    //     , m_numLEDsPerSegments(ledsPerSegment)
    //     , m_powerEnabled(true)
    // {
    //     initialize(); // todo: 
    // }

    void Display::setHeartbeatEnabled(bool isEnabled) 
    {
        m_heartbeatEnabled = isEnabled;
        if (m_heartbeatEnabled)
        {
            pinMode(m_heartbeatPin, OUTPUT);
        }
    }

    void Display::logMessage(LogLevel level, const char *format, ...) {
        if (level > m_currentLogLevel) return;  // Skip messages below the current log level

        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        Serial.print("[");
        switch (level) {
            case LOG_LEVEL_ERROR: Serial.print("E"); break;
            case LOG_LEVEL_WARN:  Serial.print("W");  break;
            case LOG_LEVEL_INFO:  Serial.print("I");  break;
            case LOG_LEVEL_DEBUG: Serial.print("D"); break;
        }
        Serial.print("] ");
        Serial.println(buffer);
    }

    void Display::initializeGlyphs(int numSegments, int ledsPerSegment) 
    {
        m_numSegments = numSegments;
        m_numLEDsPerSegments = ledsPerSegment;

        assert(m_numGlyphs > 0 && "Glyph count must be greater 0.");
        assert(m_numSegments > 0 && "Segnent count per glyph must be greater 0.");
        assert(m_numLEDsPerSegments > 0 && "LED count per segment must be greater 0.");

        m_numLEDs = m_numSegments * m_numLEDsPerSegments * m_numGlyphs;

        m_glyphs = new Glyph*[m_numGlyphs];
        for (size_t i = 0; i < m_numGlyphs; ++i) 
        {
            m_glyphs[i] = new Glyph(m_numSegments, m_numLEDsPerSegments);
            m_glyphs[i]->attach(i, m_numGlyphs);
        }

        m_LEDs = new CRGB[m_numLEDs];
		FastLED.addLeds<NEOPIXEL, constants::PinLEDs>(m_LEDs, m_numLEDs);
        FastLED.clear(true);

        m_effect = new SnakeFX(m_numLEDs, m_numLEDsPerSegments);

        logMessage(LOG_LEVEL_INFO, "Display setup: %d %d %d %d", m_numGlyphs, m_numSegments, m_numLEDsPerSegments, m_numLEDs);
    }

    void Display::setPowerEnabled(bool isEnabled) 
    {
        m_powerEnabled = isEnabled;
        if (!m_powerEnabled) 
        {
            FastLED.clear(true);
        }
    }
    
    void Display::setBrightness(int value, bool saveToEEPROM) {
        if (value > 255) {
            value = 255;
        } else if (value < 0) {
            value = 0;
        }

        // if (saveToEEPROM) {
        //     siebenuhr::Controller::getInstance()->writeToEEPROM(EEPROM_ADDRESS_BRIGHTNESS, value);
        // }

        m_nBrightness = value;
        FastLED.setBrightness(m_nBrightness);
    }

    void Display::update() 
    {
        unsigned long currentMillis = millis();

        // update glyphs
        if (m_powerEnabled && (currentMillis - m_lastUpdateTime >= 1000 / constants::FPS)) // Adjust for FPS
        {
            m_lastUpdateTime = currentMillis;

            if (m_effect != nullptr) 
            {
                m_effect->update(m_LEDs);
            }

            FastLED.show();
        }

        // update heartbeat led
        if (m_heartbeatEnabled && (currentMillis - m_lastHeartbeatTime >= m_heartbeatInterval)) {
            m_lastHeartbeatTime = currentMillis;
            m_heartbeatState = !m_heartbeatState;
            digitalWrite(m_heartbeatPin, m_heartbeatState);
        }
    }

}