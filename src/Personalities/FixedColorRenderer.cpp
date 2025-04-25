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

    void FixedColorRenderer::update(unsigned long /*currentMillis*/)
    {
        for (int i = 0; i < m_numGlyphs; ++i)
        {
            m_glyphs[i]->setColor(m_color);
        }
    }

    void FixedColorRenderer::setColor(const CRGB& color)
    {
        m_color = color;
    }
}