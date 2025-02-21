#pragma once

#include "siebenuhr_core.h"

namespace siebenuhr_core
{
    class Effect
    {
        public:
            virtual void initialize() = 0;
            virtual void update(unsigned long currentMillis) = 0;

            void attach(int offset, CRGB *LEDs)
            {
                m_glyphOffset = offset;
                m_LEDs = LEDs;
            }

        protected:
            int m_glyphOffset = 0;
            CRGB *m_LEDs = nullptr;
    };
}