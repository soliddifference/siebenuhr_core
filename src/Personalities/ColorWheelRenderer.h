#pragma once
#include "IDisplayRenderer.h"

namespace siebenuhr_core
{
    class ColorWheelRenderer : public IDisplayRenderer
    {
    public:
        ColorWheelRenderer(const CRGB& color);
        void initialize(Glyph** glyphs, int numGlyphs) override;
        void update(unsigned long currentMillis) override;

        void onGlyphChange(Glyph* glyph) override;

        // Color-related methods
        bool supportsColor() const override { return true; }
        void setColor(const CRGB& color) override;
        CRGB getColor() const override { return CHSV(m_hueStartingAngle, 255, 255); }

        const char* getName() const override { return "ColorWheel"; }

    private:
        uint8_t m_hue = 0;
        uint8_t m_hueStartingAngle = 0;
        CRGB m_color;
    };
}