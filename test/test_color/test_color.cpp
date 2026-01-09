// Tests for siebenuhr_core::Color class
// Focus: initialization, RGB/hue conversion, edge cases
#include <unity.h>
#include <cmath>
#include "siebenuhr_color.h"

using namespace siebenuhr_core;

void setUp(void) {}
void tearDown(void) {}

// ============================================================================
// Initialization & Conversion
// ============================================================================

void test_default_is_black() {
    Color c;
    CRGB rgb = c.getCRGB();
    TEST_ASSERT_EQUAL_UINT8(0, rgb.r);
    TEST_ASSERT_EQUAL_UINT8(0, rgb.g);
    TEST_ASSERT_EQUAL_UINT8(0, rgb.b);
}

void test_crgb_roundtrip() {
    // CRGB -> Color -> CRGB should preserve values
    CRGB input(100, 150, 200);
    CRGB output = Color::fromCRGB(input).getCRGB();
    TEST_ASSERT_EQUAL_UINT8(input.r, output.r);
    TEST_ASSERT_EQUAL_UINT8(input.g, output.g);
    TEST_ASSERT_EQUAL_UINT8(input.b, output.b);
}

// ============================================================================
// Hue
// ============================================================================

void test_primary_hues() {
    // Red=0°, Green=120°, Blue=240°
    CRGB red = Color::fromHue(0.0f).getCRGB();
    CRGB green = Color::fromHue(120.0f).getCRGB();
    CRGB blue = Color::fromHue(240.0f).getCRGB();
    
    TEST_ASSERT_EQUAL_UINT8(255, red.r);
    TEST_ASSERT_EQUAL_UINT8(255, green.g);
    TEST_ASSERT_EQUAL_UINT8(255, blue.b);
}

void test_hue_wraps_at_360() {
    // 360° should equal 0° (both red)
    CRGB at360 = Color::fromHue(360.0f).getCRGB();
    CRGB at0 = Color::fromHue(0.0f).getCRGB();
    TEST_ASSERT_EQUAL_UINT8(at0.r, at360.r);
    TEST_ASSERT_EQUAL_UINT8(at0.g, at360.g);
    TEST_ASSERT_EQUAL_UINT8(at0.b, at360.b);
}

void test_adjust_hue_wraps_positive() {
    Color c = Color::fromHue(350.0f);
    c.adjustHueBy(20.0f);  // 350 + 20 = 370 -> 10
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 10.0f, c.getHue());
}

void test_adjust_hue_wraps_negative() {
    Color c = Color::fromHue(5.0f);
    c.adjustHueBy(-15.0f);  // 5 - 15 = -10 -> 350
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 350.0f, c.getHue());
}

void test_get_hue_from_rgb() {
    // Verify getHue() extracts correct hue from known RGB values
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 0.0f, Color(1.0f, 0.0f, 0.0f).getHue());    // Red
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 120.0f, Color(0.0f, 1.0f, 0.0f).getHue());  // Green
    TEST_ASSERT_FLOAT_WITHIN(2.0f, 240.0f, Color(0.0f, 0.0f, 1.0f).getHue());  // Blue
}

// ============================================================================
// Brightness
// ============================================================================

void test_brightness_scales_output() {
    Color full = Color::fromHue(0.0f, 1.0f);
    Color half = Color::fromHue(0.0f, 0.5f);
    
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 1.0f, full.getBrightness());
    TEST_ASSERT_FLOAT_WITHIN(0.05f, 0.5f, half.getBrightness());
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_rgb_values_clamped() {
    // Out-of-range values should clamp to [0, 255]
    Color c(2.0f, -0.5f, 1.0f);
    CRGB rgb = c.getCRGB();
    TEST_ASSERT_EQUAL_UINT8(255, rgb.r);  // 2.0 -> 1.0 -> 255
    TEST_ASSERT_EQUAL_UINT8(0, rgb.g);    // -0.5 -> 0.0 -> 0
    TEST_ASSERT_EQUAL_UINT8(255, rgb.b);
}

// ============================================================================
// Test Runner
// ============================================================================

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    RUN_TEST(test_default_is_black);
    RUN_TEST(test_crgb_roundtrip);
    RUN_TEST(test_primary_hues);
    RUN_TEST(test_hue_wraps_at_360);
    RUN_TEST(test_adjust_hue_wraps_positive);
    RUN_TEST(test_adjust_hue_wraps_negative);
    RUN_TEST(test_get_hue_from_rgb);
    RUN_TEST(test_brightness_scales_output);
    RUN_TEST(test_rgb_values_clamped);
    
    return UNITY_END();
}
