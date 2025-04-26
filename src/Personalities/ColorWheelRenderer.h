#pragma once
#include "IDisplayRenderer.h"

namespace siebenuhr_core
{
    class ColorWheelRenderer : public IDisplayRenderer
    {
    public:
        ColorWheelRenderer(const CRGB& color);
        void initialize(Glyph** glyphs, int numGlyphs) override;
        void update(unsigned long currentMillis, int hours, int minutes) override;

        // Color-related methods
        bool supportsColor() const override { return true; }
        void setColor(const CRGB& color) override;
        CRGB getColor() const override { return m_color; }

    private:
        uint8_t m_hue = 0;
        CRGB m_color;
    };
}