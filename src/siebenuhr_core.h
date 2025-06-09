#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "siebenuhr_logger.h"

#define SIEBENUHR_CORE_VERSION "1.0.10"

namespace siebenuhr_core
{
    namespace constants 
    {
        // SolidDifference Board GPIO
        constexpr int LED2_PIN = 22; // next to the BH1750 sensor
        constexpr int LED3_PIN = 19; // next to the INA219 sensor
        constexpr int LED4_PIN = 23; // next to user button

        constexpr int LED_HEARTBEAT_PIN = LED2_PIN;
        constexpr int LED_GLYPH_PIN = 21;

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

        // notification
        constexpr CRGB NotificationColor = CRGB(255, 0, 0);
        constexpr int NotificationBrightness = 255;

        // colors
        constexpr CRGB DEFAULT_COLOR = CRGB(0, 255, 217);
        constexpr CRGB WHITE = CRGB(255, 255, 255);
        constexpr CRGB BLACK = CRGB(0, 0, 0);

        // sensors
        constexpr int SensorReadInterval = 2000;
    }

    enum ClockType {
        CLOCK_TYPE_REGULAR = 0,
        CLOCK_TYPE_MINI
    };

    enum PersonalityType {
        PERSONALITY_SOLIDCOLOR = 0,
        PERSONALITY_COLORWHEEL,
        PERSONALITY_RAINBOW,
        PERSONALITY_MOSAIK,
        PERSONALITY_GLITTER,
        PERSONALITY_END
    };

    struct LEDAnimationState {
        CRGB startColor;
        CRGB targetColor;
        unsigned long startTime;
        unsigned long duration;
        bool isActive;
    };

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
}
