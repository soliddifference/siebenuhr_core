#pragma once

#include <Arduino.h>
#include <FastLED.h>

#define SIEBENUHR_CORE_VERSION "1.0.10"

namespace siebenuhr_core
{
    namespace constants 
    {
        // SolidDifference Board GPIO
        constexpr int LED1_PIN = 22;
        constexpr int LED2_PIN = 19;
        constexpr int LED3_PIN = 23;
        constexpr int GLYPH_LED_PIN = 21;

        constexpr int USER_BUTTON_PIN = 33;
        constexpr int BOOT_BUTTON_PIN = 0;

        constexpr int SDA_PIN = 14;
        constexpr int SCL_PIN = 13;        

        constexpr int ROT_ENC_A_PIN = 26;
        constexpr int ROT_ENC_B_PIN = 27;
        constexpr int ROT_ENC_BUTTON_PIN = 18;

        constexpr int BUTTON_DEBOUNCE_DELAY = 50;    // debounce time; increase if bouncing seen


        // default clock setting
        constexpr int GlyphCount = 4;
        constexpr int SegmentCount = 7;

        constexpr int FPS = 26;
        constexpr int DefaultBrightness = 120;

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

    enum PersonalityType {
        PERSONALITY_SOLIDCOLOR = 0,
        PERSONALITY_COLORWHEEL,
        PERSONALITY_RAINBOW,
        PERSONALITY_MOSAIK,
        PERSONALITY_END
    };

    // struct PersonalityMenu_t {
    //     PersonalityType type;
    //     String name;
    //     String notification;
    // };

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
