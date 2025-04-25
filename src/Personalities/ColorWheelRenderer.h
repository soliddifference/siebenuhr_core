#pragma once
#include "IDisplayRenderer.h"

namespace siebenuhr_core
{
    class ColorWheelRenderer : public IDisplayRenderer
    {
    public:
        void initialize(Glyph** glyphs, int numGlyphs) override;
        void setText(const std::string& text) override;
        void update(unsigned long currentMillis) override;

    private:
        Glyph** m_glyphs = nullptr;
        int m_numGlyphs = 0;
        std::string m_text;
        uint8_t m_hue = 0;
    };
}