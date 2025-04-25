#include <Arduino.h>

#include "FixedColorRenderer.h"

namespace siebenuhr_core
{
    FixedColorRenderer::FixedColorRenderer(const CRGB& color)
    : m_color(color) 
    {
        
    }

    void FixedColorRenderer::initialize(Glyph** glyphs, int numGlyphs)
    {
        m_glyphs = glyphs;
        m_numGlyphs = numGlyphs;
    }

    void FixedColorRenderer::setText(const std::string& text)
    {
        m_text = text;
        for (int i = 0; i < m_numGlyphs; ++i)
        {
            if (i < static_cast<int>(m_text.length()))
                m_glyphs[i]->setAscii(m_text[i]);
            else
                m_glyphs[i]->setAscii(' ');
        }
    }

    void FixedColorRenderer::update(unsigned long currentMillis)
    {
        for (size_t i = 0; i < m_numGlyphs; ++i) 
        {
            auto glyph = m_glyphs[i];
            glyph->resetLEDS();

            for (size_t i = 0; i < glyph->getNumSegments(); ++i) 
            {
                if (glyph->getSegmentState(i)) 
                {
                    auto segmentLEDs = glyph->getSegmentLEDs(i);
                    for (size_t j = 0; j < glyph->getLEDsPerSegment(); ++j)
                    {
                        segmentLEDs[j] = m_color;
                    }
                }
            }
        }
    }

    void FixedColorRenderer::setColor(const CRGB& color)
    {
        m_color = color;
    }
}