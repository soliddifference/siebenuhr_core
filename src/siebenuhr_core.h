#pragma once

namespace siebenuhr_core::constants {
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