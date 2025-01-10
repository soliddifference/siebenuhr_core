#pragma once

#include "siebenuhr_core.h"
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

    enum ClockType {
        CLOCK_TYPE_MINI,
        CLOCK_TYPE_REGULAR
    };

    class Display 
    {
    public:
        static Display* getInstance();

        void initialize(ClockType clockType, int numGlyphs);
        void update();

        void setPowerEnabled(bool isEnabled);
        void setHeartbeatEnabled(bool isEnabled);
        void setBrightness(int value, bool saveToEEPROM = true);

        void logMessage(LogLevel level, const char *format, ...);

    private:
        Display() = default;
        ~Display() = default;

        void initializeGlyphs(int numSegments, int ledsPerSegment);

        static Display* s_instance;

        LogLevel m_currentLogLevel {LOG_LEVEL_DEBUG} ;

        ClockType m_clockType;
        unsigned long m_lastUpdateTime = 0;

        // Heartbeat
        bool m_heartbeatEnabled = false;
        int m_heartbeatPin = constants::PinHeartbeat;
        unsigned long m_lastHeartbeatTime = 0;
        unsigned long m_heartbeatInterval = 1000;
        bool m_heartbeatState = false;

        // Glyphs
        bool m_powerEnabled;
        int m_nBrightness;
        
        int m_numGlyphs;
        int m_numSegments;
        int m_numLEDsPerSegments;
        int m_numLEDs;
        Glyph** m_glyphs;
       	CRGB *m_LEDs;

        SnakeFX *m_effect;
    };
}