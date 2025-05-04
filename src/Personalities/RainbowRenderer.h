#pragma once

#include "IDisplayRenderer.h"
#include <FastLED.h>

namespace siebenuhr_core
{
    class RainbowRenderer : public IDisplayRenderer
    {
    public:
        RainbowRenderer();
        void initialize(Glyph** glyphs, int numGlyphs) override;
        void update(unsigned long currentMillis) override;

        // Color-related methods
        bool supportsColor() const override { return false; } // Rainbow has its own color logic

        const char* getName() const override { return "Rainbow"; }

    private:
        uint8_t m_hue = 0;
        unsigned long m_lastUpdateTime = 0;
        const unsigned long m_updateInterval = 50; // ms between color updates
    };
} 