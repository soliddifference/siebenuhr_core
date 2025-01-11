#pragma once

#include "siebenuhr_core.h"
#include "siebenuhr_effect.h"

namespace siebenuhr_core
{
    class Glyph 
    {
    public:
        Glyph(int numSegments, int numLEDsPerSegments);
        ~Glyph() = default;

        void attach(int glyphID, int glyphCount);
        void update(unsigned long currentMillis, CRGB *LEDs);

        void setEffect(Effect *effect);

    private:        
        int m_numLEDS;
        CRGB m_colorBase;

        int m_glyphID;
        int m_glyphOffset;

        int m_numSegments;
        int m_numLEDsPerSegments;
        
        Effect *m_effect;
    };
}