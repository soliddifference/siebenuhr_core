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
        virtual void update() = 0;

        // Text-related methods
        virtual std::string getText() const { return m_text; }
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

        // Color-related methods
        virtual bool supportsColor() const { return false; }
        virtual CRGB getColor() const { return CRGB::Black; }
        virtual void setColor(const CRGB& color) {}

        virtual const char* getName() const = 0;

        // Called when a glyph's state changes (e.g., ASCII value changes)
        virtual void onGlyphChange(Glyph* glyph) {}

        void activate() {
            for (int i = 0; i < m_numGlyphs; ++i)
            {
                m_glyphs[i]->setRenderer(this);
            }
        }

        void deactivate() {
            for (int i = 0; i < m_numGlyphs; ++i)
            {
                m_glyphs[i]->setRenderer(nullptr);
            }
        }

        Glyph** m_glyphs = nullptr;
        int m_numGlyphs = 0;
        
        std::string m_text;
    };
}