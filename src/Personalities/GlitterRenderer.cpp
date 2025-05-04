#include <Arduino.h>

#include "GlitterRenderer.h"

namespace siebenuhr_core
{
    GlitterRenderer::GlitterRenderer(const CRGB& color)
    : m_color(color) 
    {
        
    }

    void GlitterRenderer::initialize(Glyph** glyphs, int numGlyphs)
    {
        m_glyphs = glyphs;
        m_numGlyphs = numGlyphs;
    }

    void GlitterRenderer::update(unsigned long currentMillis)
    {
        for (size_t i = 0; i < m_numGlyphs; ++i) 
        {
            auto glyph = m_glyphs[i];
            glyph->resetLEDS();

            size_t ledsPerSegment = glyph->getLEDsPerSegment();                    

            for (size_t segment = 0; segment < glyph->getNumSegments(); ++segment) 
            {
                if (glyph->getSegmentState(segment)) 
                {
                    auto segmentStates = glyph->getSegmentAnimationStates(segment);
                    for (size_t led = 0; led < glyph->getLEDsPerSegment(); ++led)
                    {
                        if (random(100) < 10) 
                        {
                            segmentStates[led].isActive = true;
                            segmentStates[led].startColor = m_color;
                            segmentStates[led].targetColor = constants::BLACK;
                            segmentStates[led].startTime = millis();
                            segmentStates[led].duration = 2000;
                        }
                    }
                }
            }
        }    
    }

    void GlitterRenderer::setColor(const CRGB& color)
    {
        m_color = color;
    }
}