#pragma once

#include <Arduino.h>
#include <FastLED.h>

#include "siebenuhr_logger.h"

#define SIEBENUHR_CORE_VERSION "1.1.0"

namespace siebenuhr_core
{
    namespace constants 
    {
        // SolidDifference Board GPIO - PWM LEDs (names match board silk screen)
        constexpr int LED1_PIN = 22;             // LED 1 - near BH1750 (calibration)
        constexpr int LED2_PIN = 19;             // LED 2 - near Boot Button (decrement)
        constexpr int LED3_PIN = 23;             // LED 3 - near User Button (increment)
        constexpr int LED_HEARTBEAT_PIN = 5;     // Heartbeat LED (orange)
        constexpr int LED_GLYPH_PIN = 21;        // DOUT - FastLED RGB strip

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
        constexpr int RegularLedsPerSegment = 17;
        // constexpr int RegularLedsPerSegment = 11;

        // notification
        constexpr CRGB NotificationColor = CRGB(0, 255, 217);
        constexpr int NotificationBrightness = 255;

        // colors
        constexpr CRGB DEFAULT_COLOR = CRGB(0, 255, 217);
        constexpr CRGB WHITE = CRGB(255, 255, 255);
        constexpr CRGB BLACK = CRGB(0, 0, 0);

        // sensors
        #ifndef SENSOR_READ_INTERVAL_MS
        #define SENSOR_READ_INTERVAL_MS 10000
        #endif
        constexpr int SensorReadInterval = SENSOR_READ_INTERVAL_MS;
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

    // Brightness: non-linear scaling for better control
    // available to both PlatformIO and ESPHome versions
    
    // Get appropriate step size based on current brightness level
    // High brightness (>100): coarse steps (10)
    // Medium brightness (20-100): medium steps (5)  
    // Low brightness (<20): fine steps (1)
    inline int getBrightnessStep(int currentBrightness)
    {
        if (currentBrightness > 100) return 10;
        if (currentBrightness > 20) return 5;
        return 1;
    }
}
