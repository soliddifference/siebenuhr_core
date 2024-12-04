# siebenuhr_core

The `siebenuhr_core` library is the heart of the Siebenuhr LED clock. It provides reusable and maintainable components for driving the clock's functionality. This library is designed to support integration with Home Assistant via ESPHome as well as standalone firmware for users without a Home Assistant setup.

## Features

- **LED Topology Management**: Define and manage the layout of LEDs to display glyphs and animations.
- **Color Transitions**: Smoothly transition colors for visually appealing effects.
- **Glyph Rendering**: Handle the display of predefined or custom glyphs on the clock face.
- **Text Handling**: Render and manage text output for the clock display.

## Getting Started

This repository is a work in progress. Additional documentation and usage examples will be added as the project evolves.

### Repository Structure

```
siebenuhr_core/
├── include/          # Public header files
├── src/              # Core source files
├── examples/         # Example usage (to be added)
├── tests/            # Unit tests (to be added)
└── library.json      # PlatformIO library metadata
```

### Requirements

- **Platform**: ESP32
- **Framework**: Arduino (PlatformIO compatibility)

## Usage

### With PlatformIO

Add the following to your `platformio.ini`:

```ini
lib_deps =
    https://github.com/<your-username>/siebenuhr_core.git
```

### With ESPHome

Include the library via `external_components` in your ESPHome configuration:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/<your-username>/siebenuhr_core.git
      ref: main
```

## Contributing

Contributions are welcome! Please fork this repository and submit a pull request with your changes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---

**To be continued** with:
- Detailed usage examples
- API documentation
- Testing guidelines
```
