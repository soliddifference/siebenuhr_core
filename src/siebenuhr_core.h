#pragma once

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
}
