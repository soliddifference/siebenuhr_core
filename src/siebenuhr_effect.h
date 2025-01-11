#pragma once

#include "siebenuhr_core.h"

namespace siebenuhr_core
{
    class Effect
    {
        public:
            virtual void initialize() = 0;
            virtual void update(unsigned long currentMillis, CRGB *LEDs) = 0;

            void setGlyphOffset(int offset)
            {
                m_glyphOffset = offset;
            }

        protected:
            int m_glyphOffset = 0;
    };
}