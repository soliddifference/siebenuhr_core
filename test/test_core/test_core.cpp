// Tests for siebenuhr_core.h
// Focus: public constants, utility functions, type definitions
#include <unity.h>
#include "siebenuhr_core.h"

using namespace siebenuhr_core;

void setUp(void) {}
void tearDown(void) {}

// ============================================================================
// Public Interface - Constants
// These tests verify the expected public API is present and sensible
// ============================================================================

void test_clock_types_defined() {
    // Verify ClockType enum values exist
    TEST_ASSERT_EQUAL(0, CLOCK_TYPE_REGULAR);
    TEST_ASSERT_EQUAL(1, CLOCK_TYPE_MINI);
}

void test_personality_types_complete() {
    // Verify all expected personality types exist in sequence
    TEST_ASSERT_EQUAL(0, PERSONALITY_SOLIDCOLOR);
    TEST_ASSERT_EQUAL(1, PERSONALITY_COLORWHEEL);
    TEST_ASSERT_EQUAL(2, PERSONALITY_RAINBOW);
    TEST_ASSERT_EQUAL(3, PERSONALITY_MOSAIK);
    TEST_ASSERT_EQUAL(4, PERSONALITY_GLITTER);
    TEST_ASSERT_EQUAL(5, PERSONALITY_END);  // Sentinel value
}

void test_display_constants() {
    // Core display parameters
    TEST_ASSERT_EQUAL(4, constants::GlyphCount);
    TEST_ASSERT_EQUAL(7, constants::SegmentCount);
    TEST_ASSERT_TRUE(constants::FPS > 0);
    TEST_ASSERT_TRUE(constants::DefaultBrightness > 0);
    TEST_ASSERT_TRUE(constants::DefaultBrightness <= 255);
}

void test_led_counts() {
    // Mini vs Regular clock LED counts
    TEST_ASSERT_TRUE(constants::MiniLedsPerSegment > 0);
    TEST_ASSERT_TRUE(constants::RegularLedsPerSegment > 0);
    TEST_ASSERT_TRUE(constants::RegularLedsPerSegment > constants::MiniLedsPerSegment);
}

void test_total_led_calculation() {
    // Total LEDs = Glyphs × Segments × LEDs per segment
    int miniTotal = constants::GlyphCount * constants::SegmentCount * constants::MiniLedsPerSegment;
    int regularTotal = constants::GlyphCount * constants::SegmentCount * constants::RegularLedsPerSegment;
    
    TEST_ASSERT_EQUAL(4 * 7 * 4, miniTotal);    // 112 LEDs for mini
    TEST_ASSERT_EQUAL(4 * 7 * 17, regularTotal); // 476 LEDs for regular
}

// ============================================================================
// Utility Functions
// ============================================================================

void test_clamp_boundaries() {
    // Clamp should constrain values to range
    TEST_ASSERT_EQUAL(0, clamp(-50, 0, 100));
    TEST_ASSERT_EQUAL(50, clamp(50, 0, 100));
    TEST_ASSERT_EQUAL(100, clamp(150, 0, 100));
}

void test_brightness_step_thresholds() {
    // Non-linear step sizes: fine control at low brightness
    TEST_ASSERT_EQUAL(1, getBrightnessStep(10));   // Low: fine
    TEST_ASSERT_EQUAL(5, getBrightnessStep(50));   // Medium: medium
    TEST_ASSERT_EQUAL(10, getBrightnessStep(150)); // High: coarse
}

void test_brightness_step_boundaries() {
    // Verify boundary conditions
    TEST_ASSERT_EQUAL(1, getBrightnessStep(1));    // Min brightness
    TEST_ASSERT_EQUAL(1, getBrightnessStep(20));   // Boundary: still fine
    TEST_ASSERT_EQUAL(5, getBrightnessStep(21));   // Just above threshold
    TEST_ASSERT_EQUAL(5, getBrightnessStep(100));  // Boundary: still medium
    TEST_ASSERT_EQUAL(10, getBrightnessStep(101)); // Just above threshold
    TEST_ASSERT_EQUAL(10, getBrightnessStep(255)); // Max brightness
}

// ============================================================================
// LEDAnimationState Structure
// ============================================================================

void test_animation_state_fields() {
    LEDAnimationState state = {};
    state.isActive = true;
    state.duration = 500;
    state.startTime = 1000;
    
    TEST_ASSERT_TRUE(state.isActive);
    TEST_ASSERT_EQUAL(500, state.duration);
}

// ============================================================================
// Test Runner
// ============================================================================

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    // Public interface
    RUN_TEST(test_clock_types_defined);
    RUN_TEST(test_personality_types_complete);
    RUN_TEST(test_display_constants);
    RUN_TEST(test_led_counts);
    RUN_TEST(test_total_led_calculation);
    
    // Utilities
    RUN_TEST(test_clamp_boundaries);
    RUN_TEST(test_brightness_step_thresholds);
    RUN_TEST(test_brightness_step_boundaries);
    
    // Types
    RUN_TEST(test_animation_state_fields);
    
    return UNITY_END();
}
