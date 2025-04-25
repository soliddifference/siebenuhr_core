#include <Arduino.h>
#include "RainbowRenderer.h"

namespace siebenuhr_core
{
    RainbowRenderer::RainbowRenderer()
    {
    }

    void RainbowRenderer::initialize(Glyph** glyphs, int numGlyphs)
    {
        m_glyphs = glyphs;
        m_numGlyphs = numGlyphs;
    }

    void RainbowRenderer::setText(const std::string& text)
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

    void RainbowRenderer::update(unsigned long currentMillis)
    {
        if (currentMillis - m_lastUpdateTime >= m_updateInterval)
        {
            m_lastUpdateTime = currentMillis;
            
            // Very small increment for smooth color transition
            m_hue = (m_hue + 1) % 255; // Increment by 1 for very smooth transition

            for (int glyphIndex = 0; glyphIndex < m_numGlyphs; ++glyphIndex)
            {
                Glyph* glyph = m_glyphs[glyphIndex];
                
                // Calculate base hue for this glyph, offset by glyph position
                uint8_t baseHue = (m_hue + (glyphIndex * 30)) % 255;

                // For each segment in the glyph
                for (int segmentIndex = 0; segmentIndex < glyph->getNumSegments(); ++segmentIndex)
                {
                    if (glyph->isSegmentActive(segmentIndex))
                    {
                        // Calculate hue for this segment, with a small offset
                        uint8_t segmentHue = (baseHue + (segmentIndex * 10)) % 255;
                        
                        // Set color for this segment
                        CRGB color = CHSV(segmentHue, 255, 255);
                        glyph->setSegmentColor(segmentIndex, color);
                    }
                }
            }
        }
    }
} 