#pragma once

#include "siebenuhr_glyph.h"

namespace siebenuhr_core
{
    class Display 
    {
    public:
        Display(int numGlyphs, int numSegments, int ledsPerSegment);
        ~Display() = default;

    private:
        int m_numGlyphs;
        Glyph** m_glyphs;
    };
}