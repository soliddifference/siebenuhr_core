#include <Arduino.h>

#include "siebenuhr_display.h"

#include "ColorWheelRenderer.h"

namespace siebenuhr_core
{
    ColorWheelRenderer::ColorWheelRenderer(const CRGB& color)
    {
        setColor(color);
    }

    void ColorWheelRenderer::initialize(Glyph** glyphs, int numGlyphs)
    {
        m_glyphs = glyphs;
        m_numGlyphs = numGlyphs;
    }

    void ColorWheelRenderer::calculateCurrentHueAndColor()
    {
        int hours, minutes;
        Display::getInstance()->getTime(hours, minutes);

        // legacy version from siebenuhr v1.0
        int sec_of_day = hours * 3600 + minutes * 60;
        m_hue = (int)(m_hueStartingAngle + ((float)sec_of_day / (float)86400) * 255) % 255;
        m_color = CHSV(m_hue, 255, 255);;
    }

    void ColorWheelRenderer::onGlyphChange(Glyph* glyph)
    {
        if (glyph == nullptr)
            return;

        calculateCurrentHueAndColor();

        for (size_t i = 0; i < glyph->getNumSegments(); ++i) 
        {
            auto segmentLEDs = glyph->getSegmentLEDs(i);
            auto segmentStates = glyph->getSegmentAnimationStates(i);

            for (size_t j = 0; j < glyph->getLEDsPerSegment(); ++j)
            {
                segmentStates[j].isActive = true;
                segmentStates[j].startColor = segmentLEDs[j];
                segmentStates[j].targetColor = glyph->getSegmentState(i) ? m_color : CRGB::Black;
                segmentStates[j].startTime = millis();
                segmentStates[j].duration = glyph->getSegmentState(i) ? 250 : 100;
            }
        }
    }

    void ColorWheelRenderer::update(unsigned long currentMillis)
    {
        // as the color changes slowly over time the glyphs not in an active animation state will be updated to the current color
        calculateCurrentHueAndColor();

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
    }

    void ColorWheelRenderer::setColor(const CRGB& color)
    {
        CHSV color_hsv = rgb2hsv_approximate(color);
        m_hueStartingAngle = color_hsv.hue;
    }
}
