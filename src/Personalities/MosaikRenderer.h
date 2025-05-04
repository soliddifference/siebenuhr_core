#pragma once

#include "IDisplayRenderer.h"
#include <FastLED.h>

namespace siebenuhr_core
{
    class MosaikRenderer : public IDisplayRenderer
    {
    public:
        MosaikRenderer();
        void initialize(Glyph** glyphs, int numGlyphs) override;
        void update(unsigned long currentMillis) override;

        // Color-related methods
        bool supportsColor() const override { return false; } // Mosaik has its own color logic

        const char* getName() const override { return "Mosaik"; }

    private:
        uint8_t m_hue = 0;
        unsigned long m_lastUpdateTime = 0;
        const unsigned long m_updateInterval = 50; // ms between color updates
    };
} 