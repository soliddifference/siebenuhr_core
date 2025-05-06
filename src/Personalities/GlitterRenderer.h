#pragma once

#include "IDisplayRenderer.h"

#include <FastLED.h>

namespace siebenuhr_core
{
    class GlitterRenderer : public IDisplayRenderer
    {
    public:
        GlitterRenderer(const CRGB& color);
        void initialize(Glyph** glyphs, int numGlyphs) override;
        void update() override;

        // Color-related methods
        bool supportsColor() const override { return true; }
        void setColor(const CRGB& color) override;
        CRGB getColor() const override { return m_color; }

        const char* getName() const override { return "FixedColor"; }

    private:
        CRGB m_color;
    };
}