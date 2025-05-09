#pragma once

#include "esp_log.h"

namespace siebenuhr_core {

class Logger {
public:
    static void init(const char* tag) {
        s_tag = tag;
    }

    static void error(const char* format, ...) {
        va_list args;
        va_start(args, format);
        esp_log_writev(ESP_LOG_ERROR, s_tag, format, args);
        va_end(args);
    }

    static void warn(const char* format, ...) {
        va_list args;
        va_start(args, format);
        esp_log_writev(ESP_LOG_WARN, s_tag, format, args);
        va_end(args);
    }

    static void info(const char* format, ...) {
        va_list args;
        va_start(args, format);
        esp_log_writev(ESP_LOG_INFO, s_tag, format, args);
        va_end(args);
    }

    static void debug(const char* format, ...) {
        va_list args;
        va_start(args, format);
        esp_log_writev(ESP_LOG_DEBUG, s_tag, format, args);
        va_end(args);
    }

    static void verbose(const char* format, ...) {
        va_list args;
        va_start(args, format);
        esp_log_writev(ESP_LOG_VERBOSE, s_tag, format, args);
        va_end(args);
    }

private:
    static const char* s_tag;
};

// Convenience macros
#define LOG_E(format, ...) siebenuhr_core::Logger::error(format, ##__VA_ARGS__)
#define LOG_W(format, ...) siebenuhr_core::Logger::warn(format, ##__VA_ARGS__)
#define LOG_I(format, ...) siebenuhr_core::Logger::info(format, ##__VA_ARGS__)
#define LOG_D(format, ...) siebenuhr_core::Logger::debug(format, ##__VA_ARGS__)
#define LOG_V(format, ...) siebenuhr_core::Logger::verbose(format, ##__VA_ARGS__)

} // namespace siebenuhr_core 