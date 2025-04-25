#include <Arduino.h>
#include <FastLED.h>

#include "ColorWheelRenderer.h"

namespace siebenuhr_core
{
    void ColorWheelRenderer::initialize(Glyph** glyphs, int numGlyphs)
    {
        m_glyphs = glyphs;
        m_numGlyphs = numGlyphs;
    }

    void ColorWheelRenderer::setText(const std::string& text)
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

    void ColorWheelRenderer::update(unsigned long /*currentMillis*/)
    {
        // Update hue on each call to create the color wheel effect
        m_hue++;

        for (int i = 0; i < m_numGlyphs; ++i)
        {
            // Offset the hue per glyph for a rotating color pattern
            uint8_t glyphHue = m_hue + (i * (256 / m_numGlyphs));
            CRGB color = CHSV(glyphHue, 255, 255);
            m_glyphs[i]->setColor(color);
        }
    }
}
