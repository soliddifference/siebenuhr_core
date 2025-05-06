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

    void RainbowRenderer::update()
    {
        m_hue = (m_hue + 1) % 256;

        int ledIndex = 0;
        for (size_t i = 0; i < m_numGlyphs; ++i) 
        {
            auto glyph = m_glyphs[i];
            glyph->resetLEDS();

            if (true)
            {
                for (size_t segment = 0; segment < glyph->getNumSegments(); ++segment) 
                {
                    if (glyph->getSegmentState(segment)) 
                    {
                        auto segmentLEDs = glyph->getSegmentLEDs(segment);
                        size_t ledsPerSegment = glyph->getLEDsPerSegment();
                        
                        // Create rainbow pattern within the segment
                        for (size_t led = 0; led < ledsPerSegment; ++led)
                        {
                            // Calculate hue for this LED based on its position in the segment
                            uint8_t ledHue = (m_hue + ((led >> 1) * (256 / ledsPerSegment))) % 256;
                            segmentLEDs[led] = CHSV(ledHue, 255, 255);
                        }
                    }
                }
            }
            else
            {
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
} 