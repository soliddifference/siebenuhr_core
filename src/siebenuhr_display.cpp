#include <Arduino.h>

#include "siebenuhr_core.h"
#include "siebenuhr_display.h"

namespace siebenuhr_core
{
    Display::Display(int numGlyphs, int numSegments, int ledsPerSegment)
        : m_numGlyphs(numGlyphs)
        , m_numSegments(numSegments)
        , m_numLEDsPerSegments(ledsPerSegment)
        , m_powerEnabled(true)
    {
        initialize(); // todo: 
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

    void Display::initialize()
    {
        Serial.begin(115200);

        initializeGlyphs();

        // set saved configuration
        m_nBrightness = constants::DefaultBrightness;
       	// TODO: _nBrightness = _inst->readFromEEPROM(EEPROM_ADDRESS_BRIGHTNESS);
        setBrightness(m_nBrightness);
    }

    void Display::initializeGlyphs() 
    {
        assert(m_numGlyphs > 0 && "Glyph count must be greater 0.");
        assert(m_numSegments > 0 && "Segnent count per glyph must be greater 0.");
        assert(m_numLEDsPerSegments > 0 && "LED count per segment must be greater 0.");

        ESP_LOGW("lib_name", "Message for print!!!"); 

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
        if (m_powerEnabled && (currentMillis - m_lastUpdateTime >= 1000 / constants::FPS)) // Adjust for FPS
        {
            m_lastUpdateTime = currentMillis;

            if (m_effect != nullptr) 
            {
                m_effect->update(m_LEDs);
            }

            FastLED.show();
        }
    }

}