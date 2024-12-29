#include <Arduino.h>

#include "siebenuhr_core.h"
#include "siebenuhr_display.h"

/*
--- <4 ---
|        |
2u       3d
|        |
--- 1>---
|        |
6u       0u
|        |
--- 5> ---
*/

std::vector<std::vector<SegmentConnection>> adjacencyMap = {
    {{1, false}, {3, false}},   // Segment 0 neighbors
    {{0, false}, {3, false}},   // Segment 1 neighbors
    {{4, false}},               // Segment 2 neighbors
    {{1, false}, {0, false}},   // Segment 3 neighbors
    {{2, false}},               // Segment 4 neighbors
    {{0, true}},                // Segment 5 neighbors
    {{1, true}, {2, true}}      // Segment 6 neighbors
};

std::vector<std::vector<SegmentConnection>> adjacencyMapRev = {
    {{5, false}},               // Segment 0 neighbors
    {{2, true}, {6, false}},    // Segment 1 neighbors
    {{1, true}, {6, false}},    // Segment 2 neighbors
    {{4, true}},                // Segment 3 neighbors
    {{3, true}},                // Segment 4 neighbors
    {{6, true}},                // Segment 5 neighbors
    {{5, true}}                 // Segment 6 neighbors
};

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

        m_curLEDPos = 0;
        m_LEDCols = new int[m_numLEDs];
        memset(m_LEDCols, 0, sizeof(int)*m_numLEDs); 

        m_LEDs = new CRGB[m_numLEDs];
		FastLED.addLeds<NEOPIXEL, constants::PinLEDs>(m_LEDs, m_numLEDs);
        FastLED.clear(true);

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

    size_t Display::getLEDIndex(size_t segment, size_t ledPos) {
        if (segment % 2 == 0) { // Even segments: bottom-up order
            return segment * m_numLEDsPerSegments + ledPos;
        } else { // Odd segments: top-down order
            return segment * m_numLEDsPerSegments + (m_numLEDsPerSegments - 1 - ledPos);
        }
    }

    void Display::selectNextSegment(std::vector<std::vector<SegmentConnection>> &map) 
    {
        // Choose the next segment based on adjacency
        const auto &connections = map[m_curSegment];
        const auto &nextConnection = connections[std::rand() % connections.size()];
        m_curSegment = nextConnection.nextSegment;
        m_directionUp = nextConnection.entryDirection;

        if (m_directionUp) 
        {
            m_curLEDPos = 0;
        } 
        else
        {
            m_curLEDPos = m_numLEDsPerSegments - 1;
        }
    }

    void Display::update() 
    {
        unsigned long currentMillis = millis();
        if (m_powerEnabled && (currentMillis - m_lastUpdateTime >= 1000 / constants::FPS)) // Adjust for FPS
        {
            m_lastUpdateTime = currentMillis;

            // Dim all LEDs
            for (size_t i = 0; i < m_numLEDs; ++i) 
            {
                m_LEDCols[i] = m_LEDCols[i] >> 1; // Smooth fade
            }

            // Light up the current LED
            size_t ledIndex = getLEDIndex(m_curSegment, m_curLEDPos);
            m_LEDCols[ledIndex] = 255;

            // Move within the current segment
            if (m_directionUp) {
                m_curLEDPos++;
                if (m_curLEDPos >= m_numLEDsPerSegments) 
                { 
                    // Exit the segment at the top
                    selectNextSegment(adjacencyMap);
                }
            } else {
                if (m_curLEDPos > 0) {
                    m_curLEDPos--;
                } else {
                    // Exit the segment at the bottom
                    selectNextSegment(adjacencyMapRev);
                }
            }

            // Update the physical LEDs
            for (size_t i = 0; i < m_numLEDs; ++i) 
            {
                m_LEDs[i] = CRGB(m_LEDCols[i], m_LEDCols[i], m_LEDCols[i]);
            }

            FastLED.show();
        }
    }

}