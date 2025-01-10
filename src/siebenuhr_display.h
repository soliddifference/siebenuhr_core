#pragma once

#include "siebenuhr_glyph.h"
#include <FastLED.h>

#include "FX/snake.h"

#include <vector>

namespace siebenuhr_core
{
    enum LogLevel {
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARN,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG
    };

    class Display 
    {
    public:
        Display(int numGlyphs, int numSegments, int ledsPerSegment);
        ~Display() = default;

        void initialize();
        void update();

        void setPowerEnabled(bool isEnabled);
        void setBrightness(int value, bool saveToEEPROM = true);

        void logMessage(LogLevel level, const char *format, ...);

    private:
        void initializeGlyphs();

        LogLevel m_currentLogLevel {LOG_LEVEL_DEBUG} ;

        bool m_powerEnabled;
        unsigned long m_lastUpdateTime = 0;

        int m_nBrightness;

        int m_numGlyphs;
        int m_numSegments;
        int m_numLEDsPerSegments;
        int m_numLEDs;

        Glyph** m_glyphs;
       	CRGB *m_LEDs;
        int *m_LEDCols;

        SnakeFX *m_effect;
    };
}