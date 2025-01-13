#include "siebenuhr_glyph.h"

namespace siebenuhr_core
{
    Glyph::Glyph(int numSegments, int numLEDsPerSegments)
        : m_numSegments(numSegments), m_numLEDsPerSegments(numLEDsPerSegments)

    {
        m_numLEDS = m_numSegments * m_numLEDsPerSegments;
    }

    void Glyph::attach(int glyphID, int glyphCount)
    {
        m_glyphID = glyphID;
        m_glyphOffset = (glyphCount - m_glyphID - 1) * m_numSegments * m_numLEDsPerSegments;
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
        else
        {
            char_id = (char_id+1)%127;

            for (size_t i = 0; i < m_numSegments * m_numLEDsPerSegments; ++i) 
            {
                LEDs[m_glyphOffset + i].r = 0;
                LEDs[m_glyphOffset + i].g = 0;
                LEDs[m_glyphOffset + i].b = 0;
            }

            for (size_t i = 0; i < m_numSegments; ++i) 
            {
                if (ASCII_TABLE[char_id][i] == 1)
                {
                    int segmentOffset = i*m_numLEDsPerSegments;
                    for (size_t e = 0; e < m_numLEDsPerSegments; ++e) 
                    {
                        LEDs[m_glyphOffset + i].r = 255;
                        LEDs[m_glyphOffset + i].g = 255;
                        LEDs[m_glyphOffset + i].b = 255;
                    }                    
                }
            }
        }
    }

}