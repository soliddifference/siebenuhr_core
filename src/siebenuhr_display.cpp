#include "siebenuhr_display.h"

namespace siebenuhr_core
{
    Display::Display(int numGlyphs, int numSegments, int ledsPerSegment)
        : m_numGlyphs(numGlyphs)
    {
        m_glyphs = new Glyph*[m_numGlyphs];
        for (size_t i = 0; i < m_numGlyphs; ++i) 
        {
            m_glyphs[i] = new Glyph(numSegments, ledsPerSegment);
        }
    }
}