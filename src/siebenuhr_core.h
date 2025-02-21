#pragma once

#include <Arduino.h>
#include <FastLED.h>

namespace siebenuhr_core
{
    namespace constants 
    {
        constexpr int PinHeartbeat = 2;    
        constexpr int PinLEDs = 16; // QuinLED Board LED1 

        constexpr int GlyphCount = 4;
        constexpr int SegmentCount = 7;

        // default clock setting
        constexpr int FPS = 26;
        constexpr int DefaultBrightness = 255;

        // MINI CLOCK
        constexpr int MiniLedsPerSegment = 4;

        // REGULAR CLOCK
        constexpr int RegularLedsPerSegment = 11;

        // colors
        constexpr CRGB WHITE = CRGB(255, 255, 255);
        constexpr CRGB BLACK = CRGB(0, 0, 0);
    }

    template <typename T>
    T clamp(T value, T minValue, T maxValue) 
    {
        if (value > maxValue) {
            return maxValue;
        } else if (value < minValue) {
            return minValue;
        }
        return value;
    }

    enum LogLevel {
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARN,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG
    };

    extern void setLogLevel(LogLevel level);
    extern void logMessage(LogLevel level, const char *format, ...);
}
