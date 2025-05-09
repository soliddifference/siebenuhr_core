#pragma once

#include <Arduino.h>
#include "esp_log.h"

// Log level definitions using enum class for type safety
enum class CoreLogLevel {
    NONE = 0,
    ERROR = 1,
    WARN = 2,
    INFO = 3,
    CONFIG = 4,
    DEBUG = 5,
    VERBOSE = 6
};

#define CORE_LOG_COLOR_BLACK "30"
#define CORE_LOG_COLOR_RED "31"
#define CORE_LOG_COLOR_GREEN "32"
#define CORE_LOG_COLOR_YELLOW "33"
#define CORE_LOG_COLOR_BLUE "34"
#define CORE_LOG_COLOR_MAGENTA "35"
#define CORE_LOG_COLOR_CYAN "36"
#define CORE_LOG_COLOR_WHITE "37"

#define CORE_LOG_COLOR(COLOR) "\033[0;" COLOR "m"
#define CORE_LOG_RESET_COLOR "\033[0m"

#define CORE_LOG_COLOR_E CORE_LOG_COLOR(CORE_LOG_COLOR_RED)
#define CORE_LOG_COLOR_W CORE_LOG_COLOR(CORE_LOG_COLOR_YELLOW)
#define CORE_LOG_COLOR_I CORE_LOG_COLOR(CORE_LOG_COLOR_GREEN)
#define CORE_LOG_COLOR_D CORE_LOG_COLOR(CORE_LOG_COLOR_BLUE)
#define CORE_LOG_COLOR_V CORE_LOG_COLOR(CORE_LOG_COLOR_CYAN)

namespace siebenuhr_core {

class Logger {
public:
    static void init(const char* tag) {
        s_tag = tag;
        s_log_level = CoreLogLevel::INFO; // Default to INFO level
        // Set default log level for all components
        esp_log_level_set("*", ESP_LOG_INFO);

        // Set specific log level for our tag
        esp_log_level_set(s_tag, ESP_LOG_VERBOSE);
        
        // Log initialization with color
        info("Logger initialized with tag: %s", s_tag);
    }

    static void setLogLevel(CoreLogLevel level) {
        s_log_level = level;

        String levelName = "NONE";
        switch (level) {
            case CoreLogLevel::ERROR:
                levelName = "ERROR";
                break;
            case CoreLogLevel::WARN:
                levelName = "WARN";
                break;
            case CoreLogLevel::INFO:
                levelName = "INFO";
                break;
            case CoreLogLevel::CONFIG:
                levelName = "CONFIG";
                break;
            case CoreLogLevel::DEBUG:
                levelName = "DEBUG";
                break;
            case CoreLogLevel::VERBOSE:
                levelName = "VERBOSE";
                break;
        }

        info("Log level set to: %s(%d)", levelName.c_str(), static_cast<int>(level));
    }

    static CoreLogLevel getLogLevel() {
        return s_log_level;
    }

    static void error(const char* format, ...) {
        if (s_log_level < CoreLogLevel::ERROR) return;
        va_list args;
        va_start(args, format);
        printf("[E]%s[%s] ", CORE_LOG_COLOR_E, s_tag);
        vprintf(format, args);
        printf("%s\n", CORE_LOG_RESET_COLOR);
        va_end(args);
    }

    static void warn(const char* format, ...) {
        if (s_log_level < CoreLogLevel::WARN) return;
        va_list args;
        va_start(args, format);
        printf("[W]%s[%s] ", CORE_LOG_COLOR_W, s_tag);
        vprintf(format, args);
        printf("%s\n", CORE_LOG_RESET_COLOR);
        va_end(args);
    }

    static void info(const char* format, ...) {
        if (s_log_level < CoreLogLevel::INFO) return;
        va_list args;
        va_start(args, format);
        printf("[I]%s[%s] ", CORE_LOG_COLOR_I, s_tag);
        vprintf(format, args);
        printf("%s\n", CORE_LOG_RESET_COLOR);
        va_end(args);
    }

    static void config(const char* format, ...) {
        if (s_log_level < CoreLogLevel::CONFIG) return;
        va_list args;
        va_start(args, format);
        printf("[C]%s[%s] ", CORE_LOG_COLOR_I, s_tag);
        vprintf(format, args);
        printf("%s\n", CORE_LOG_RESET_COLOR);
        va_end(args);
    }

    static void debug(const char* format, ...) {
        if (s_log_level < CoreLogLevel::DEBUG) return;
        va_list args;
        va_start(args, format);
        printf("[D]%s[%s] ", CORE_LOG_COLOR_D, s_tag);
        vprintf(format, args);
        printf("%s\n", CORE_LOG_RESET_COLOR);
        va_end(args);
    }

    static void verbose(const char* format, ...) {
        if (s_log_level < CoreLogLevel::VERBOSE) return;
        va_list args;
        va_start(args, format);
        printf("[V]%s[%s] ", CORE_LOG_COLOR_V, s_tag);
        vprintf(format, args);
        printf("%s\n", CORE_LOG_RESET_COLOR);
        va_end(args);
    }

private:
    static const char* s_tag;
    static CoreLogLevel s_log_level;
};

// Convenience macros
#define LOG_E(format, ...) siebenuhr_core::Logger::error(format, ##__VA_ARGS__)
#define LOG_W(format, ...) siebenuhr_core::Logger::warn(format, ##__VA_ARGS__)
#define LOG_I(format, ...) siebenuhr_core::Logger::info(format, ##__VA_ARGS__)
#define LOG_C(format, ...) siebenuhr_core::Logger::config(format, ##__VA_ARGS__)
#define LOG_D(format, ...) siebenuhr_core::Logger::debug(format, ##__VA_ARGS__)
#define LOG_V(format, ...) siebenuhr_core::Logger::verbose(format, ##__VA_ARGS__)

} // namespace siebenuhr_core 