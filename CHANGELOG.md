# Changelog

All notable changes to the ESP32-S3-Box3 Display Audio Photo project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-10-25

### Added

#### Core Features
- Complete ESP32-S3-Box3 multimedia application
- LVGL-based graphical user interface with tabbed navigation
- SPIFFS filesystem integration for file storage
- Support for JPEG image viewing with hardware acceleration
- WAV audio playback through built-in speaker
- Text file reader for .txt files
- Audio recording capability using built-in microphone
- Volume control (0-100%)
- Display brightness control (0-100%)

#### User Interface
- Three-tab interface:
  - **Files Tab**: Browse and open files stored in SPIFFS
  - **Recording Tab**: Record and playback audio
  - **Settings Tab**: Adjust volume and brightness
- Touch-responsive file browser
- Full-screen image viewer
- Text display with scrolling
- Audio playback controls
- Recording status indicators

#### Documentation
- Comprehensive README.md with:
  - Detailed feature descriptions
  - Hardware and software requirements
  - Step-by-step installation guide
  - Building and flashing instructions
  - Usage guide with examples
  - Troubleshooting section
  - Technical specifications
- QUICK_START.md for rapid setup
- CONTRIBUTING.md with contribution guidelines
- API.md with detailed API documentation
- Inline code documentation with Doxygen-style comments

#### Sample Content
- Sample JPEG images:
  - esp_logo.jpg (Espressif logo)
  - Death Star.jpg (Star Wars themed)
  - Millenium Falcon.jpg (Star Wars themed)
- Sample audio:
  - imperial_march.wav (Star Wars Imperial March theme)
- Sample text:
  - Readme.txt (Welcome message)

#### Build System
- CMake-based build configuration
- ESP-IDF component integration
- Automatic SPIFFS image generation
- Custom partition table for 16MB flash
- Pre-configured sdkconfig for ESP32-S3-Box3

#### Configuration
- Optimized settings for ESP32-S3-Box3:
  - 240MHz CPU frequency
  - 16MB Flash support
  - 8MB Octal PSRAM enabled
  - LVGL9 configuration
  - SPIFFS with 3MB storage
- Component dependencies via idf_component.yml:
  - esp-box-3 BSP
  - esp_jpeg decoder

### Technical Details

#### Hardware Support
- ESP32-S3-Box3 development board
- 320x240 IPS LCD display
- Capacitive touch panel (GT911)
- ES8311 audio codec
- Built-in speaker (2W, 8Ω)
- MEMS microphone

#### Software Stack
- ESP-IDF v5.0+ compatibility
- LVGL v9.x graphics library
- SPIFFS filesystem
- I2S audio interface
- Hardware JPEG decoder
- FreeRTOS

#### Memory Configuration
- Application partition: 1MB
- SPIFFS partition: ~3MB
- NVS partition: 24KB
- PHY init partition: 4KB

### Project Structure
```
esp-audio-video/
├── main/                  # Application source code
├── doc/                   # Documentation assets
├── spiffs_content/        # Files for SPIFFS
├── CMakeLists.txt         # Project build config
├── partitions.csv         # Partition table
├── sdkconfig files        # ESP-IDF configuration
└── Documentation files    # README, guides, etc.
```

### Dependencies
- ESP-IDF v5.0 or later
- ESP-BSP (esp-box-3) component
- esp_jpeg component
- LVGL v9.x (included in BSP)

### Known Limitations
- JPEG images: Recommended max 800x600 pixels
- Audio format: Only WAV (PCM 16-bit) supported
- SPIFFS size: Limited to ~3MB total
- Single recording file (overwritten on new recording)

## [Unreleased]

### Planned Features
- MP3 audio support
- PNG image support
- Multiple recording slots
- File management (delete, rename)
- Wi-Fi connectivity
- Over-the-air (OTA) updates
- SD card support for expanded storage
- Video playback (basic animation)
- Touch gestures (swipe, pinch)
- Customizable themes

### Potential Improvements
- Reduce memory usage
- Faster JPEG decoding
- Better error handling
- More audio sample rates
- Landscape/portrait orientation support
- Screen saver
- Battery monitoring (if on battery)

---

## Version History

- **1.0.0** (2025-10-25): Initial release

## Links

- [Repository](https://github.com/DanielStuttgart/esp-audio-video)
- [Issues](https://github.com/DanielStuttgart/esp-audio-video/issues)
- [Pull Requests](https://github.com/DanielStuttgart/esp-audio-video/pulls)
- [ESP32-S3 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [ESP-BSP Repository](https://github.com/espressif/esp-bsp)

## Credits

This project is based on the [ESP-BSP display_audio_photo example](https://github.com/espressif/esp-bsp/tree/master/examples/display_audio_photo) by Espressif Systems.

Special thanks to:
- Espressif Systems for ESP-IDF and ESP-BSP
- LVGL team for the graphics library
- The ESP32 community

## License

- Main application code: CC0-1.0 (Public Domain)
- Supporting code: Apache License 2.0
- See individual files for specific license information
