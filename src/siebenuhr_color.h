#pragma once

#include <FastLED.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <tuple>

namespace siebenuhr_core {

    /*
     * Color class
     * 
     * This class represents a color in the RGB color space.
     * It provides methods to convert between different color spaces and to get the hue, brightness, and RGB values.
     * 
     * Features:
     * - Uses full float precision internally
     * - Avoids rounding/quantization problems during hue/brightness adjustments
     * - Provides optional uint8_t conversion for output purposes
     */

    class Color {
    public:
        Color() : m_r(0.0f), m_g(0.0f), m_b(0.0f) {}

        Color(float r, float g, float b)
            : m_r(clamp01(r)), m_g(clamp01(g)), m_b(clamp01(b)) {}

        static Color fromCRGB(const CRGB& color) 
        {
            return Color(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
        }

        static Color fromRGB8(int r, int g, int b) 
        {
            return Color(r / 255.0f, g / 255.0f, b / 255.0f);
        }

        static Color fromHue(float hueDegrees, float brightness = 1.0f) 
        {
            hueDegrees = std::fmod(hueDegrees, 360.0f);
            if (hueDegrees < 0.0f) hueDegrees += 360.0f;

            float c = brightness;
            float x = c * (1.0f - std::fabs(std::fmod(hueDegrees / 60.0f, 2.0f) - 1.0f));
            float r = 0, g = 0, b = 0;

            if (hueDegrees < 60)       { r = c; g = x; b = 0; }
            else if (hueDegrees < 120) { r = x; g = c; b = 0; }
            else if (hueDegrees < 180) { r = 0; g = c; b = x; }
            else if (hueDegrees < 240) { r = 0; g = x; b = c; }
            else if (hueDegrees < 300) { r = x; g = 0; b = c; }
            else                       { r = c; g = 0; b = x; }

            return Color(r, g, b);
        }

        float getHue() const 
        {
            float maxC = std::max({ m_r, m_g, m_b });
            float minC = std::min({ m_r, m_g, m_b });
            float delta = maxC - minC;

            if (delta < 1e-6f) return 0.0f; // undefined hue for grayscale

            float hue;
            if (maxC == m_r) hue = fmodf((m_g - m_b) / delta, 6.0f);
            else if (maxC == m_g) hue = (m_b - m_r) / delta + 2.0f;
            else                 hue = (m_r - m_g) / delta + 4.0f;

            hue *= 60.0f;
            if (hue < 0.0f) hue += 360.0f;
            return hue;
        }

        void setHue(float hueDegrees) {
            float brightness = getBrightness();
            *this = fromHue(hueDegrees, brightness);
        }

        void adjustHueBy(float deltaDegrees) {
            float currentHue = getHue();
            float newHue = std::fmod(currentHue + deltaDegrees, 360.0f);
            if (newHue < 0.0f) newHue += 360.0f;
            setHue(newHue);
        }

        float getBrightness() const 
        {
            return std::max({ m_r, m_g, m_b });
        }

        void setRGB(float r, float g, float b) 
        {
            m_r = clamp01(r); m_g = clamp01(g); m_b = clamp01(b);
        }

        CRGB getCRGB() const 
        {
            return CRGB(             
                static_cast<uint8_t>(std::round(m_r * 255.0f)),
                static_cast<uint8_t>(std::round(m_g * 255.0f)),
                static_cast<uint8_t>(std::round(m_b * 255.0f))
            );
        }

    private:
        
        float m_r, m_g, m_b;

        static float clamp01(float x) {
            if (x < 0.0f) return 0.0f;
            if (x > 1.0f) return 1.0f;
            return x;
        }
    };

}