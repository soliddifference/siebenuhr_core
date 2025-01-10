#include "siebenuhr_core.h"

namespace siebenuhr_core
{
    LogLevel g_logLevel = LOG_LEVEL_DEBUG;

    void setLogLevel(LogLevel level)
    {
        g_logLevel = level;
    }

    void logMessage(LogLevel level, const char *format, ...) {
        if (level > g_logLevel) return;  // Skip messages below the current log level

        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        Serial.print("[");
        switch (level) {
            case LOG_LEVEL_ERROR: Serial.print("E"); break;
            case LOG_LEVEL_WARN:  Serial.print("W");  break;
            case LOG_LEVEL_INFO:  Serial.print("I");  break;
            case LOG_LEVEL_DEBUG: Serial.print("D"); break;
        }
        Serial.print("] ");
        Serial.println(buffer);
    }

}
