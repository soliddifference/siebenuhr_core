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

    void FixedColorRenderer::onGlyphChange(Glyph* glyph)
    {
        if (glyph == nullptr)
            return;

        auto startTime = millis();
        for (size_t i = 0; i < glyph->getNumSegments(); ++i) 
        {
            auto segmentLEDs = glyph->getSegmentLEDs(i);
            auto segmentStates = glyph->getSegmentAnimationStates(i);

            for (size_t j = 0; j < glyph->getLEDsPerSegment(); ++j)
            {
                segmentStates[j].isActive = true;
                segmentStates[j].startColor = segmentLEDs[j];
                segmentStates[j].targetColor = glyph->getSegmentState(i) ? m_color : CRGB::Black;
                segmentStates[j].startTime = startTime;
                segmentStates[j].duration = glyph->getSegmentState(i) ? 250 : 100;
            }
        }
    }

    void FixedColorRenderer::update()
    {
        if (m_forceUpdate)
        {
            for (size_t i = 0; i < m_numGlyphs; ++i) 
            {
                auto glyph = m_glyphs[i];
                for (size_t i = 0; i < glyph->getNumSegments(); ++i) 
                {
                    if (glyph->getSegmentState(i))
                    {
                        auto segmentLEDs = glyph->getSegmentLEDs(i);
                        auto segmentAnimationStates = glyph->getSegmentAnimationStates(i);
                        for (size_t j = 0; j < glyph->getLEDsPerSegment(); ++j) 
                        {
                            if (!segmentAnimationStates[j].isActive)
                            {   
                                segmentLEDs[j] = m_color;
                            }
                        }
                    }
                }
            }
            m_forceUpdate = false;
        }
    }

    void FixedColorRenderer::setColor(const CRGB& color)
    {
        m_color = color;
        m_forceUpdate = true;
    }
}