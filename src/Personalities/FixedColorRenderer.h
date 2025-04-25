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
        void setText(const std::string& text) override;
        void update(unsigned long currentMillis) override;

        // Color-related methods
        bool supportsColor() const override { return true; }
        void setColor(const CRGB& color) override;
        CRGB getColor() const override { return m_color; }

    private:
        Glyph** m_glyphs = nullptr;
        int m_numGlyphs = 0;
        std::string m_text;
        CRGB m_color;
    };
}