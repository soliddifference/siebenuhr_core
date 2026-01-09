// Mock FastLED.h for native testing
// Provides CRGB and related types without hardware dependencies
#pragma once

#include <cstdint>
#include <algorithm>

// Forward declarations
struct CRGB;
struct CHSV;

// CRGB - RGB color (constexpr-compatible for native tests)
struct CRGB {
    uint8_t r, g, b;
    
    constexpr CRGB() : r(0), g(0), b(0) {}
    constexpr CRGB(uint8_t r_, uint8_t g_, uint8_t b_) : r(r_), g(g_), b(b_) {}
    constexpr CRGB(uint32_t colorcode) 
        : r((colorcode >> 16) & 0xFF)
        , g((colorcode >> 8) & 0xFF)
        , b(colorcode & 0xFF) {}
    
    constexpr bool operator==(const CRGB& other) const {
        return r == other.r && g == other.g && b == other.b;
    }
    constexpr bool operator!=(const CRGB& other) const { return !(*this == other); }
    
    // Named colors - declared here, defined after struct is complete
    static const CRGB Black;
    static const CRGB White;
    static const CRGB Red;
    static const CRGB Green;
    static const CRGB Blue;
};

// Define static colors after CRGB is complete
inline constexpr CRGB CRGB::Black{0, 0, 0};
inline constexpr CRGB CRGB::White{255, 255, 255};
inline constexpr CRGB CRGB::Red{255, 0, 0};
inline constexpr CRGB CRGB::Green{0, 255, 0};
inline constexpr CRGB CRGB::Blue{0, 0, 255};

// CHSV - HSV color
struct CHSV {
    uint8_t h, s, v;
    CHSV() : h(0), s(0), v(0) {}
    CHSV(uint8_t h, uint8_t s, uint8_t v) : h(h), s(s), v(v) {}
};

// lerp8by8 - linear interpolation
inline uint8_t lerp8by8(uint8_t a, uint8_t b, uint8_t frac) {
    int result = a + (((int)(b - a) * frac) >> 8);
    return (uint8_t)result;
}

// Mock FastLED controller class
class CFastLED {
public:
    template<typename CHIPSET, uint8_t DATA_PIN, uint8_t COLOR_ORDER>
    static void addLeds(CRGB* data, int nLedsOrOffset, int nLedsIfOffset = 0) {
        (void)data; (void)nLedsOrOffset; (void)nLedsIfOffset;
    }
    
    static void setBrightness(uint8_t scale) { _brightness = scale; }
    static uint8_t getBrightness() { return _brightness; }
    static void setDither(uint8_t dither) { (void)dither; }
    static uint16_t getFPS() { return 30; }
    static void show() {}
    static void clear(bool writeData = false) { (void)writeData; }
    
private:
    static uint8_t _brightness;
};

inline uint8_t CFastLED::_brightness = 255;

extern CFastLED FastLED;

// LED chipset types (stubs)
#define WS2812 0
#define WS2812B 1
#define GRB 0
#define RGB 1
