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
        virtual void setText(const std::string& text)
        {
            m_text = text;
            for (int i = 0; i < m_numGlyphs; ++i)
            {
                if (i < static_cast<int>(m_text.length()))
                    m_glyphs[i]->setAscii(m_text[i]);
                else
                    m_glyphs[i]->setAscii(' ');
            }
        }

        virtual void update(unsigned long currentMillis, int hours, int minutes) = 0;

        // Color-related methods
        virtual bool supportsColor() const { return false; }
        virtual void setColor(const CRGB& color) {}
        virtual CRGB getColor() const { return CRGB::Black; }

        Glyph** m_glyphs = nullptr;
        int m_numGlyphs = 0;
        
        std::string m_text;
    };
}