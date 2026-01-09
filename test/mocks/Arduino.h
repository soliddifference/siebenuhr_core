// Mock Arduino.h for native testing
// Provides minimal stubs for Arduino types and functions
#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>

// Arduino types
typedef uint8_t byte;
typedef bool boolean;

// Pin modes
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

// Digital values
#define HIGH 1
#define LOW 0

// Math macros
#ifndef PI
#define PI 3.14159265358979323846
#endif

// Stub functions
inline void pinMode(uint8_t pin, uint8_t mode) { (void)pin; (void)mode; }
inline void digitalWrite(uint8_t pin, uint8_t val) { (void)pin; (void)val; }
inline int digitalRead(uint8_t pin) { (void)pin; return LOW; }
inline int analogRead(uint8_t pin) { (void)pin; return 0; }
inline void analogWrite(uint8_t pin, int val) { (void)pin; (void)val; }
inline void delay(unsigned long ms) { (void)ms; }
inline void delayMicroseconds(unsigned int us) { (void)us; }

// Time - test harness can override these
static unsigned long _mock_millis = 0;
inline unsigned long millis() { return _mock_millis; }
inline unsigned long micros() { return _mock_millis * 1000; }
inline void _set_mock_millis(unsigned long ms) { _mock_millis = ms; }

// String class (simplified)
class String {
public:
    String() : _str() {}
    String(const char* s) : _str(s ? s : "") {}
    String(const std::string& s) : _str(s) {}
    String(int val) : _str(std::to_string(val)) {}
    
    const char* c_str() const { return _str.c_str(); }
    size_t length() const { return _str.length(); }
    bool operator==(const String& other) const { return _str == other._str; }
    String& operator+=(const String& other) { _str += other._str; return *this; }
    String operator+(const String& other) const { return String(_str + other._str); }
    
private:
    std::string _str;
};

// Serial stub
class MockSerial {
public:
    void begin(unsigned long baud) { (void)baud; }
    void print(const char* s) { printf("%s", s); }
    void print(int n) { printf("%d", n); }
    void println(const char* s = "") { printf("%s\n", s); }
    void println(int n) { printf("%d\n", n); }
};

extern MockSerial Serial;
