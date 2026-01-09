// Tests for ASCII_TABLE and DIGIT lookups in siebenuhr_glyph.h
// These are header-only constants - no .cpp linkage needed
#include <unity.h>
#include "siebenuhr_glyph.h"

using namespace siebenuhr_core;

void setUp(void) {}
void tearDown(void) {}

// ============================================================================
// ASCII Table - Digit Patterns (0-9)
// ============================================================================

void test_digit_segment_counts() {
    // Each digit 0-9 should have between 2-7 active segments
    for (int digit = 0; digit <= 9; digit++) {
        int ascii = '0' + digit;
        int activeCount = 0;
        for (int seg = 0; seg < 7; seg++) {
            if (ASCII_TABLE[ascii][seg] != 0) activeCount++;
        }
        TEST_ASSERT_TRUE_MESSAGE(activeCount >= 2, "Digit should have at least 2 segments");
        TEST_ASSERT_TRUE_MESSAGE(activeCount <= 7, "Digit should have at most 7 segments");
    }
}

void test_digit_8_all_segments() {
    // '8' should light all 7 segments
    for (int seg = 0; seg < 7; seg++) {
        TEST_ASSERT_EQUAL_INT(1, ASCII_TABLE['8'][seg]);
    }
}

void test_digit_1_two_segments() {
    // '1' should have exactly 2 segments
    int count = 0;
    for (int seg = 0; seg < 7; seg++) {
        if (ASCII_TABLE['1'][seg] != 0) count++;
    }
    TEST_ASSERT_EQUAL_INT(2, count);
}

void test_digits_match_DIGIT_array() {
    // ASCII_TABLE['0'-'9'] should match DIGIT[0-9]
    for (int d = 0; d <= 9; d++) {
        for (int seg = 0; seg < 7; seg++) {
            TEST_ASSERT_EQUAL_INT(DIGIT[d][seg], ASCII_TABLE['0' + d][seg]);
        }
    }
}

// ============================================================================
// ASCII Table - Special Characters
// ============================================================================

void test_space_is_blank() {
    // ' ' (space) should have no segments
    for (int seg = 0; seg < 7; seg++) {
        TEST_ASSERT_EQUAL_INT(0, ASCII_TABLE[' '][seg]);
    }
}

void test_minus_is_middle_segment() {
    // '-' typically lights only the middle segment (index varies by layout)
    int count = 0;
    for (int seg = 0; seg < 7; seg++) {
        if (ASCII_TABLE['-'][seg] != 0) count++;
    }
    TEST_ASSERT_EQUAL_INT(1, count);  // Only middle segment
}

// ============================================================================
// ASCII Table - Letters
// ============================================================================

void test_common_letters_have_patterns() {
    // Letters commonly used in clock displays should have patterns
    const char* letters = "AbCdEFgHiJLnoPqrtUy";
    for (int i = 0; letters[i] != '\0'; i++) {
        int ascii = letters[i];
        int count = 0;
        for (int seg = 0; seg < 7; seg++) {
            if (ASCII_TABLE[ascii][seg] != 0) count++;
        }
        TEST_ASSERT_TRUE_MESSAGE(count > 0, "Letter should have at least 1 segment");
    }
}

// ============================================================================
// ASCII Table - Bounds
// ============================================================================

void test_ascii_table_size() {
    // Table should cover ASCII 0-126
    // Just verify we can access without crash
    int sum = 0;
    for (int i = 0; i < 127; i++) {
        for (int seg = 0; seg < 7; seg++) {
            sum += ASCII_TABLE[i][seg];
        }
    }
    TEST_ASSERT_TRUE(sum > 0);  // Some segments should be set
}

// ============================================================================
// Test Runner
// ============================================================================

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    // Digits
    RUN_TEST(test_digit_segment_counts);
    RUN_TEST(test_digit_8_all_segments);
    RUN_TEST(test_digit_1_two_segments);
    RUN_TEST(test_digits_match_DIGIT_array);
    
    // Special characters
    RUN_TEST(test_space_is_blank);
    RUN_TEST(test_minus_is_middle_segment);
    
    // Letters
    RUN_TEST(test_common_letters_have_patterns);
    
    // Bounds
    RUN_TEST(test_ascii_table_size);
    
    return UNITY_END();
}
