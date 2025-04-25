#pragma once

#include "IDisplayRenderer.h"

#include <FastLED.h>

namespace siebenuhr_core
{
    class FixedColorRenderer : public IDisplayRenderer
    {
    public:
        FixedColorRenderer(const CRGB& color);
        void initialize(Glyph** glyphs, int numGlyphs) override;
        void update(unsigned long currentMillis) override;

        // Color-related methods
        bool supportsColor() const override { return true; }
        void setColor(const CRGB& color) override;
        CRGB getColor() const override { return m_color; }

    private:
        CRGB m_color;
    };
}