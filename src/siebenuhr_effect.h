#pragma once

#include <FastLED.h>

namespace siebenuhr_core
{
    class Effect
    {
        public:
            virtual void initialize() = 0;
            virtual void update(CRGB *m_LEDs) = 0;
    };
}