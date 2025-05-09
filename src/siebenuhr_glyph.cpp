#include "siebenuhr_glyph.h"
#include "IDisplayRenderer.h"

namespace siebenuhr_core
{
    Glyph::Glyph(int numSegments, int numLEDsPerSegments)
    : m_numSegments(numSegments)
    , m_numLEDsPerSegments(numLEDsPerSegments)
    , m_numLEDSPerGlyph(numSegments * numLEDsPerSegments)
    , m_LEDs(nullptr)
    , m_animationStates(nullptr)
    // , m_effect(nullptr)
    , m_curAscii(' ')
    {
    }

    void Glyph::attach(int glyphID, int glyphCount, CRGB *LEDs, LEDAnimationState* animationStates)
    {
        if (LEDs == nullptr || animationStates == nullptr)
        {
            LOG_E("glyph:attach => Null LED or animation state pointer");
            return;
        }
    
        m_glyphID = glyphID;
        m_glyphOffset = (glyphCount - m_glyphID - 1) * m_numLEDSPerGlyph;
        m_LEDs = LEDs;
        m_animationStates = animationStates;
    
        LOG_I("Glyph %d attached at offset %d with %d LEDs.", m_glyphID, m_glyphOffset, m_numLEDSPerGlyph);
    }

    void Glyph::setRenderer(IDisplayRenderer* renderer) 
    { 
        m_renderer = renderer; 
        if (m_renderer != nullptr) {
            m_renderer->onGlyphChange(this);
        }
    }

    void Glyph::setAscii(char value) 
    {
        m_curAscii = value;
        m_curAscii = clamp(m_curAscii, 0, 126);

        // Notify renderer of the change
        if (m_renderer != nullptr) {
            m_renderer->onGlyphChange(this);
        }
    }

    void Glyph::resetLEDS()
    {
        for (size_t i = 0; i < m_numLEDSPerGlyph; ++i) 
        {
            m_LEDs[m_glyphOffset + i] = constants::BLACK;
            m_animationStates[m_glyphOffset + i].isActive = false;
        }
    }
}