# SiebenuhrCore Library

The `siebenuhr_core` library is the heart of the Siebenuhr LED clock. It provides reusable and maintainable components for driving the clock's functionality. This library is designed to support integration with Home Assistant via ESPHome as well as standalone firmware for users without a Home Assistant setup.

Dependent projects:
- https://github.com/soliddifference/siebenuhr_esphome
- https://github.com/soliddifference/siebenuhr

## Features

- **LED Topology Management**: Define and manage the layout of LEDs to display glyphs and animations.
- **Color Transitions**: Smoothly transition colors for visually appealing effects.
- **Glyph Rendering**: Handle the display of predefined or custom glyphs on the clock face.
- **Text Handling**: Render and manage text output for the clock display.

## Hardware 

*(both Miniclock and Controller boards)*

| Function | GPIO | Notes |
|----------|------|-------|
| DOUT (RGB Strip) | 21 | FastLED addressable LEDs (WS2812/SK6812) |
| LED 1 | 22 | PWM  near BH1750, for future calibration use|
| LED 2 | 19 | PWM, near Boot Button |
| LED 3 | 23 | PWM, near User Button |
| Heartbeat LED | 5 | PWM, orange, blinks every 1s |
| User Button | 33 | |
| Boot Button | 0 | |
| Rotary Encoder | 26, 27, 18 | A, B, Button |

*I2C sensors:*

- BH1750 (0x23) - Ambient light sensor
- INA219 (0x40) - Power monitoring

## Getting Started

### Requirements

- **Platform**: ESP32 (ESP32-MINI-1-N4, 4MB flash, no PSRAM)
- **Framework**: Arduino (PlatformIO compatibility)
- **Development**: Python 3.8+ with PlatformIO

### Repository Structure

```
siebenuhr_core/
├── src/              # Core library source files
│   ├── Personalities/    # Display renderers (ColorWheel, Rainbow, etc.)
│   └── FX/               # Special effects
├── test/             # Native unit tests
│   ├── mocks/            # Arduino/FastLED mocks for desktop testing
│   └── test_*/           # Test suites
├── library.json      # PlatformIO library metadata
└── platformio.ini    # Test configuration (not used by dependents)
```

## Usage

### With PlatformIO

Add the following to your `platformio.ini`:

```ini
lib_deps =
    https://github.com/soliddifference/siebenuhr_core.git
```

### With ESPHome

Include the library via `libraries` in your ESPHome configuration:

```yaml
libraries:
    - https://github.com/soliddifference/siebenuhr_core.git
```

### Local Repository (Dev Mode)

To work with a local version of the library during development, reference the library using a file:// path instead of a GitHub URL. This allows for faster compile and testing without requiring Git commits.

```ini
lib_deps =
  file://C:\local\path\to\the\core_package\dir\siebenuhr_core
```

or for ESPHOME:

```yaml
libraries:
    - file://C:\local\path\to\the\core_package\dir\siebenuhr_core
```

Replace the path above with the location of your locally checked-out repository.

## Development

### Setup

```bash
# Install PlatformIO
pip install -r requirements.txt

# Or with uv
uv pip install -r requirements.txt
```

### Running Tests

Tests run on desktop (native) without hardware using mocked Arduino/FastLED:

```bash
# Run all tests
pio test -e native

# Run specific test suite
pio test -e native -f test_core

# Verbose output
pio test -e native -v
```

### Build Flags

The core library uses these compile-time flags (set in `platformio.ini`):

```ini
build_flags = 
    -D SENSOR_READ_INTERVAL_MS=10000    ; Sensor polling interval in ms (default: 10000)
    -D FASTLED_DITHER_ENABLED=1         ; Enable FastLED temporal dithering (off by default)
    -D DOUBLE_CLICK_PERSONALITY_ENABLED ; Enable double-click to cycle personalities
```

| Flag | Default | Description |
|------|---------|-------------|
| `SENSOR_READ_INTERVAL_MS` | 10000 | How often to read I2C sensors (BH1750, INA219) |
| `FASTLED_DITHER_ENABLED` | off | Enable temporal dithering (can cause flicker at low brightness) |
| `DOUBLE_CLICK_PERSONALITY_ENABLED` | off | Double-click buttons to cycle display personalities |

### Runtime Configuration

These features are controlled at runtime via the controller API, allowing ESPHome and other integrations to configure them dynamically:

```cpp
controller->setAutoBrightnessEnabled(true);   // Enable BH1750 ambient light adjustment
controller->setPowerMonitoringEnabled(true);  // Enable INA219 power logging
Logger::setLogLevel(CoreLogLevel::VERBOSE);   // Enable verbose logging (FPS stats, etc.)
```

Downstream projects may use their own build flags to set defaults for these runtime options.

## Contributing

Contributions are welcome! Please fork this repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

