#include <Arduino.h>
#include <FastLED.h>

#include "ColorWheelRenderer.h"

namespace siebenuhr_core
{
    ColorWheelRenderer::ColorWheelRenderer(const CRGB& color)
    : m_color(color) 
    {
        
    }

    void ColorWheelRenderer::initialize(Glyph** glyphs, int numGlyphs)
    {
        m_glyphs = glyphs;
        m_numGlyphs = numGlyphs;
    }

    void ColorWheelRenderer::update(unsigned long currentMillis)
    {
        m_hue = (m_hue + 1) % 256;
        m_color = CHSV(m_hue, 255, 255);

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

    void ColorWheelRenderer::setColor(const CRGB& color)
    {
        m_color = color;
    }
}
