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

    void Glyph::setEffect(Effect *effect)
    {
        m_effect = effect;
        m_effect->setGlyphOffset(m_glyphOffset);
    }

    void Glyph::update(unsigned long currentMillis, CRGB *LEDs)
    {
        if (m_effect != nullptr) 
        {
            m_effect->update(currentMillis, LEDs);
        }
    }

}