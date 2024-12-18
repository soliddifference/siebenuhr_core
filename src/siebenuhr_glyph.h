#pragma once

#include <FastLED.h>

namespace siebenuhr_core
{
    class Glyph 
    {
    public:
        Glyph(int segmentCount, int ledsPerSegment);
        ~Glyph() = default;

    private:        
        int m_numLEDS;
        CRGB m_colorBase;

        int m_glyphID;
        int m_glyphOffset;
    };
}