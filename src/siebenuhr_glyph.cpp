#include "siebenuhr_glyph.h"

namespace siebenuhr_core
{
    Glyph::Glyph(int segmentCount, int ledsPerSegment)
    {
        m_numLEDS = segmentCount * ledsPerSegment;
    }
}