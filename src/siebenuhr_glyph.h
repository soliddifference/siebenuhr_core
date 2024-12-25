#pragma once

#include <FastLED.h>

namespace siebenuhr_core
{
    class Glyph 
    {
    public:
        Glyph(int numSegments, int numLEDsPerSegments);
        ~Glyph() = default;

        void attach(int glyphID, int glyphCount);

    private:        
        int m_numLEDS;
        CRGB m_colorBase;

        int m_glyphID;
        int m_glyphOffset;

        int m_numSegments;
        int m_numLEDsPerSegments;
    };
}