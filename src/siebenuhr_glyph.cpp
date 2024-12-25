#include "siebenuhr_glyph.h"

namespace siebenuhr_core
{
    Glyph::Glyph(int numSegments, int numLEDsPerSegments)
        : m_numSegments(numSegments)
        , m_numLEDsPerSegments(numLEDsPerSegments)

    {
        m_numLEDS = m_numSegments * m_numLEDsPerSegments;
    }

    void Glyph::attach(int glyphID, int glyphCount) 
    {
        m_glyphID = glyphID;
        m_glyphOffset = (glyphCount-m_glyphID-1)*m_numSegments*m_numLEDsPerSegments;
    }
}