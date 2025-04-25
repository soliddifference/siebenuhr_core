// IDisplayRenderer.h
#pragma once

#include "siebenuhr_glyph.h"
#include <FastLED.h>

#include <string>

namespace siebenuhr_core
{
    class IDisplayRenderer
    {
    public:
        virtual ~IDisplayRenderer() = default;

        virtual void initialize(Glyph** glyphs, int numGlyphs) = 0;

        // virtual void setColor(const CRGB& color) = 0;
        virtual void setText(const std::string& text) = 0;

        virtual void update(unsigned long currentMillis) = 0;

        // Color-related methods
        virtual bool supportsColor() const { return false; }
        virtual void setColor(const CRGB& color) {}
        virtual CRGB getColor() const { return CRGB::Black; }
    };
}