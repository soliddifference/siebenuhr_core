#include <Arduino.h>

#include "MosaikRenderer.h"

namespace siebenuhr_core
{
    MosaikRenderer::MosaikRenderer()
    {
    }

    void MosaikRenderer::initialize(Glyph** glyphs, int numGlyphs)
    {
        m_glyphs = glyphs;
        m_numGlyphs = numGlyphs;
    }

    void MosaikRenderer::update()
    {
        m_hue = (m_hue + 1) % 256;

        int ledIndex = 0;
        for (size_t i = 0; i < m_numGlyphs; ++i) 
        {
            auto glyph = m_glyphs[i];
            glyph->resetLEDS();

            for (size_t segment = 0; segment < glyph->getNumSegments(); ++segment) 
            {
                if (glyph->getSegmentState(segment)) 
                {
                    auto segmentLEDs = glyph->getSegmentLEDs(segment);
                    size_t ledsPerSegment = glyph->getLEDsPerSegment();
                    
                    for (size_t led = 0; led < ledsPerSegment; ++led)
                    {
                        segmentLEDs[led] = CHSV((m_hue + ledIndex) % 256, 255, 255);
                        ledIndex++;
                    }
                }
            }
        }    
    }
} 