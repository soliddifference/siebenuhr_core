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

## v1.0.10 (Apr - Dec 2025)

Feature-complete release with all personalities and controls.

- Added Rainbow, Mosaik, Glitter personalities (Apr-May)
- Button class with single-click, double-click, long-press support (May)
- Notification system with overlay rendering (May)
- Refactored controller and logger (May)
- High-precision hue manipulation for color accuracy (May)
- Device reset event detection (May)
- Accesspoint and render state management (Jun)

## v1.0.5 - v1.0.7 (Apr 2025)

Rapid iteration adding core functionality.

- v1.0.7: Rotary encoder prewiring
- v1.0.6: Sensor settings (BH1750, INA219)
- v1.0.5: ColorWheel personality

## v1.0.1 - v1.0.4 (Apr 2025)

Initial versioned releases.

- v1.0.4: ESPHome integration, color/brightness/power control
- v1.0.1: Auto-brightness support, first version macro

## v1.0.0 (Dec 2024 - Apr 2025)

Initial development and library structure.

- Library scaffolding for PlatformIO and ESPHome
- Display and Glyph classes
- ASCII character rendering
- SolidColor personality
- Snake test effect
