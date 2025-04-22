#pragma once

#include <Arduino.h>
#include <FastLED.h>

#define SIEBENUHR_CORE_VERSION "1.0.6"

namespace siebenuhr_core
{
    namespace constants 
    {
        // GPIOs
        constexpr int PinHeartbeat = 2;    
        // constexpr int PinLEDs = 16; // QuinLED Board LED1 
        constexpr int PinLEDs = 21; // SolidDifference Board LED1 

        constexpr int SDA_PIN = 14;
        constexpr int SCL_PIN = 13;
        
        // default clock setting
        constexpr int GlyphCount = 4;
        constexpr int SegmentCount = 7;

        constexpr int FPS = 26;
        constexpr int DefaultBrightness = 255;

        // MINI CLOCK
        constexpr int MiniLedsPerSegment = 4;

        // REGULAR CLOCK
        constexpr int RegularLedsPerSegment = 11;

        // colors
        constexpr CRGB WHITE = CRGB(255, 255, 255);
        constexpr CRGB BLACK = CRGB(0, 0, 0);

        // sensors
        constexpr int SensorReadInterval = 2000;
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
