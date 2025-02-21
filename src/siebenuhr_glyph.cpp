#include "siebenuhr_glyph.h"

namespace siebenuhr_core
{
    Glyph::Glyph(int numSegments, int numLEDsPerSegments)
    : m_numSegments(numSegments)
    , m_numLEDsPerSegments(numLEDsPerSegments)
    , m_numLEDSPerGlyph(numSegments * numLEDsPerSegments)
    , m_LEDs(nullptr)
    , m_effect(nullptr)
    {
    }

    void Glyph::attach(int glyphID, int glyphCount, CRGB *LEDs)
    {
        if (LEDs == nullptr)
        {
            logMessage(LOG_LEVEL_ERROR, "glyph:attach => Null LED pointer");
            return;
        }
    
        m_glyphID = glyphID;
        m_glyphOffset = (glyphCount - m_glyphID - 1) * m_numLEDSPerGlyph;
        m_LEDs = LEDs;
    
        logMessage(LOG_LEVEL_INFO, "Glyph %d attached at offset %d with %d LEDs.", m_glyphID, m_glyphOffset, m_numLEDS);
    }


    void Glyph::setEffect(Effect *effect)
    {
        m_effect = effect;
        if (!m_effect)
            return;

        m_effect->initialize();
        m_effect->setGlyphOffset(m_glyphOffset);
    }

    void Glyph::setAscii(char value) 
    {
        m_curAscii = value;
        m_curAscii = clamp(m_curAscii, 0, 126);

        logMessage(LOG_LEVEL_INFO, "glyph:setAscii => %d set char: %c", m_glyphID, (char)m_curAscii);
    }

    void Glyph::resetLEDS()
    {
        for (size_t i = 0; i < m_numLEDSPerGlyph; ++i) 
        {
            m_LEDs[m_glyphOffset + i] = constants::BLACK;
        }
    }

    void Glyph::lightUpSegment(size_t segmentIndex)
    {
        int segmentOffset = segmentIndex * m_numLEDsPerSegments;
    
        for (size_t e = 0; e < m_numLEDsPerSegments; ++e) 
        {
            m_LEDs[m_glyphOffset + segmentOffset + e] = constants::WHITE;
        }
    }    

    void Glyph::update(unsigned long currentMillis)
    {
        assert(m_LEDs != nullptr && "Attach glyph before running.");

        if (m_effect != nullptr)
        {
            m_effect->update(currentMillis, LEDs);
            return;
        }

        resetLEDS();

        for (size_t i = 0; i < m_numSegments; ++i) 
        {
            if (ASCII_TABLE[m_curAscii][i] == 1)
            {
                lightUpSegment(i);
            }
        }
    }

}