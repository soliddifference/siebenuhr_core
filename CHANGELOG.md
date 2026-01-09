# Changelog

## v1.1.0 (Jan 2026)

- Brightness scaling: non-linear steps for finer control at low brightness, minimum reduced from 5 to 1
- Double-click on buttons now cycles through display personalities
- Sensor reading interval configurable via `SENSOR_READ_INTERVAL_MS` build flag (default 10s)
- Encoder feedback LED now optional via constructor parameter
- Added test suites runnable in native (non-hw) environment using mocks
- Added platformio.ini for running tests
- Added CI workflow for automated testing
- Documented build flags and runtime configuration APIs
- Added `FASTLED_DITHER_ENABLED` build flag (disabled by default to prevent flicker at low brightness)
- Fixed LED pin mapping: heartbeat now on GPIO 5, button LEDs on correct pins
- Heartbeat LED dims to ~10% brightness, toggles every 1s
- Button LEDs light when pressed
- FPS stats logged at verbose level only
- Removed deprecated `readAndPrintPowerMonitoring()` method
- Fixed logger switch statement missing NONE case

## v1.0.0 (2025)

- initial release supporting Miniclock and Controller boards, for both ESPHome and PlatformIO
