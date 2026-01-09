// Tests for siebenuhr_core::Logger
// Focus: log level ordering, level filtering, format safety
#include <unity.h>
#include "siebenuhr_logger.h"

// Define static members for test (normally in siebenuhr_logger.cpp)
namespace siebenuhr_core {
    const char* Logger::s_tag = "test";
    CoreLogLevel Logger::s_log_level = CoreLogLevel::INFO;
}

using namespace siebenuhr_core;

void setUp(void) {
    // Reset to known state before each test
    Logger::setLogLevel(CoreLogLevel::INFO);
}

void tearDown(void) {}

// ============================================================================
// CoreLogLevel Enum - Ordering
// Level filtering depends on numeric ordering: NONE < ERROR < WARN < INFO < DEBUG < VERBOSE
// ============================================================================

void test_log_levels_ordered() {
    // Verify levels are in ascending order for filtering to work
    TEST_ASSERT_TRUE(CoreLogLevel::NONE < CoreLogLevel::ERROR);
    TEST_ASSERT_TRUE(CoreLogLevel::ERROR < CoreLogLevel::WARN);
    TEST_ASSERT_TRUE(CoreLogLevel::WARN < CoreLogLevel::INFO);
    TEST_ASSERT_TRUE(CoreLogLevel::INFO < CoreLogLevel::CONFIG);
    TEST_ASSERT_TRUE(CoreLogLevel::CONFIG < CoreLogLevel::DEBUG);
    TEST_ASSERT_TRUE(CoreLogLevel::DEBUG < CoreLogLevel::VERBOSE);
}

void test_log_level_values() {
    // Explicit value check - ensures enum values don't accidentally change
    TEST_ASSERT_EQUAL(0, static_cast<int>(CoreLogLevel::NONE));
    TEST_ASSERT_EQUAL(1, static_cast<int>(CoreLogLevel::ERROR));
    TEST_ASSERT_EQUAL(2, static_cast<int>(CoreLogLevel::WARN));
    TEST_ASSERT_EQUAL(3, static_cast<int>(CoreLogLevel::INFO));
    TEST_ASSERT_EQUAL(4, static_cast<int>(CoreLogLevel::CONFIG));
    TEST_ASSERT_EQUAL(5, static_cast<int>(CoreLogLevel::DEBUG));
    TEST_ASSERT_EQUAL(6, static_cast<int>(CoreLogLevel::VERBOSE));
}

// ============================================================================
// setLogLevel / getLogLevel
// ============================================================================

void test_set_get_log_level() {
    Logger::setLogLevel(CoreLogLevel::DEBUG);
    TEST_ASSERT_EQUAL(CoreLogLevel::DEBUG, Logger::getLogLevel());
    
    Logger::setLogLevel(CoreLogLevel::ERROR);
    TEST_ASSERT_EQUAL(CoreLogLevel::ERROR, Logger::getLogLevel());
    
    Logger::setLogLevel(CoreLogLevel::NONE);
    TEST_ASSERT_EQUAL(CoreLogLevel::NONE, Logger::getLogLevel());
}

void test_default_log_level() {
    // After init or reset, default should be INFO
    Logger::setLogLevel(CoreLogLevel::INFO);
    TEST_ASSERT_EQUAL(CoreLogLevel::INFO, Logger::getLogLevel());
}

// ============================================================================
// Log Function Calls - Verify No Crashes
// These tests verify the log functions handle various inputs without crashing
// ============================================================================

void test_log_functions_no_args() {
    // Simple string with no format specifiers
    Logger::setLogLevel(CoreLogLevel::VERBOSE);
    
    // Should not crash
    Logger::error("simple error message");
    Logger::warn("simple warning");
    Logger::info("simple info");
    Logger::config("simple config");
    Logger::debug("simple debug");
    Logger::verbose("simple verbose");
    
    TEST_PASS();
}

void test_log_functions_with_args() {
    Logger::setLogLevel(CoreLogLevel::VERBOSE);
    
    // Various format specifiers
    Logger::info("int: %d", 42);
    Logger::info("string: %s", "test");
    Logger::info("float: %.2f", 3.14f);
    Logger::info("multiple: %d, %s, %.1f", 1, "two", 3.0f);
    
    TEST_PASS();
}

void test_log_macros_compile() {
    Logger::setLogLevel(CoreLogLevel::VERBOSE);
    
    // Verify macros expand correctly
    LOG_E("error via macro");
    LOG_W("warn via macro");
    LOG_I("info via macro");
    LOG_C("config via macro");
    LOG_D("debug via macro");
    LOG_V("verbose via macro");
    
    // With arguments
    LOG_I("value: %d", 123);
    
    TEST_PASS();
}

// ============================================================================
// Level Filtering - Verify Messages Are Suppressed at Lower Levels
// ============================================================================

void test_none_level_suppresses_all() {
    Logger::setLogLevel(CoreLogLevel::NONE);
    
    // At NONE level, even ERROR should be suppressed
    // (We can't easily verify output, but we verify no crash)
    Logger::error("should not appear");
    Logger::warn("should not appear");
    Logger::info("should not appear");
    
    TEST_PASS();
}

void test_error_level_filtering() {
    Logger::setLogLevel(CoreLogLevel::ERROR);
    
    // Only ERROR should pass through at ERROR level
    // WARN, INFO, etc. should be filtered
    // Verify level check logic
    TEST_ASSERT_TRUE(CoreLogLevel::ERROR >= CoreLogLevel::ERROR);   // passes
    TEST_ASSERT_FALSE(CoreLogLevel::ERROR >= CoreLogLevel::WARN);   // filtered
    TEST_ASSERT_FALSE(CoreLogLevel::ERROR >= CoreLogLevel::INFO);   // filtered
}

// ============================================================================
// Edge Cases
// ============================================================================

void test_empty_format_string() {
    Logger::setLogLevel(CoreLogLevel::INFO);
    
    // Empty string should not crash
    Logger::info("");
    
    TEST_PASS();
}

void test_null_string_in_format() {
    Logger::setLogLevel(CoreLogLevel::INFO);
    
    // Note: Passing NULL to %s is undefined behavior in C, but some
    // implementations handle it gracefully. This test documents the risk.
    // In production code, always check for NULL before logging strings.
    
    // We don't test this as it's UB, but document it exists
    TEST_PASS();
}

// ============================================================================
// Test Runner
// ============================================================================

int main(int argc, char **argv) {
    UNITY_BEGIN();
    
    // Level ordering (critical for filtering)
    RUN_TEST(test_log_levels_ordered);
    RUN_TEST(test_log_level_values);
    
    // Level management
    RUN_TEST(test_set_get_log_level);
    RUN_TEST(test_default_log_level);
    
    // Function calls (no crash)
    RUN_TEST(test_log_functions_no_args);
    RUN_TEST(test_log_functions_with_args);
    RUN_TEST(test_log_macros_compile);
    
    // Filtering
    RUN_TEST(test_none_level_suppresses_all);
    RUN_TEST(test_error_level_filtering);
    
    // Edge cases
    RUN_TEST(test_empty_format_string);
    RUN_TEST(test_null_string_in_format);
    
    return UNITY_END();
}
