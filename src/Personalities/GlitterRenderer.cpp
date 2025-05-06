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

        srand(millis());
    }

    void GlitterRenderer::update()
    {
        unsigned long currentMillis = millis();
        for (size_t i = 0; i < m_numGlyphs; ++i) 
        {
            auto glyph = m_glyphs[i];

            for (size_t segment = 0; segment < glyph->getNumSegments(); ++segment) 
            {
                if (glyph->getSegmentState(segment)) 
                {
                    auto segmentAnimationStates = glyph->getSegmentAnimationStates(segment);
                    for (size_t led = 0; led < glyph->getLEDsPerSegment(); ++led)
                    {

                        if (!segmentAnimationStates[led].isActive)
                        {
                            int randomValue = rand() % 100;
                            if (randomValue < 1)
                            {
                                segmentAnimationStates[led].isActive = true;
                                segmentAnimationStates[led].startColor = m_color;
                                segmentAnimationStates[led].targetColor = constants::BLACK;
                                segmentAnimationStates[led].startTime = currentMillis;
                                segmentAnimationStates[led].duration = 3000;
                            }
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