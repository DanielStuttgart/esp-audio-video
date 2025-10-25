# ESP32-S3-Box3 Display Audio Photo Example

A comprehensive multimedia application for the ESP32-S3-Box3 development board, demonstrating display, touch, audio playback, and file system capabilities.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Project Structure](#project-structure)
- [Getting Started](#getting-started)
  - [Installation](#installation)
  - [Building the Project](#building-the-project)
  - [Flashing the Firmware](#flashing-the-firmware)
- [Usage](#usage)
- [Configuration](#configuration)
- [Troubleshooting](#troubleshooting)
- [Technical Details](#technical-details)
- [Contributing](#contributing)
- [License](#license)

## 🎯 Overview

This project is based on the [ESP-BSP display_audio_photo example](https://github.com/espressif/esp-bsp/tree/master/examples/display_audio_photo) and specifically configured for the **ESP32-S3-Box3** development board. It provides a rich multimedia experience with a user-friendly graphical interface built using LVGL (Light and Versatile Graphics Library).

The application allows users to:
- Browse files stored in the internal SPIFFS filesystem
- View JPEG images with hardware-accelerated decoding
- Play WAV audio files through the onboard speaker
- Read text files directly on the display
- Record and playback audio using the built-in microphone
- Adjust system settings like volume and brightness

## ✨ Features

### Display Features
- **📱 320x240 pixel IPS LCD** with capacitive touch support
- **🎨 LVGL-based UI** with smooth animations and responsive touch controls
- **🖼️ JPEG Image Viewer** - View JPEG images with hardware decoding
- **📄 Text File Reader** - Read .txt files directly on screen
- **🎛️ Tabbed Interface** - Easy navigation between Files, Recording, and Settings

### Audio Features
- **🎵 WAV Audio Playback** - Play 16-bit WAV files
- **🎤 Audio Recording** - Record audio clips using the built-in microphone
- **🔊 Volume Control** - Adjustable speaker volume (0-100%)
- **📊 Audio Format Support** - Mono/Stereo, various sample rates

### File System Features
- **💾 SPIFFS Integration** - 3MB storage for files
- **📂 File Browser** - Touch-based file navigation
- **📝 Multiple File Types** - Support for .jpg, .wav, and .txt files
- **🔄 Easy Content Updates** - Pre-loaded sample content

## 🛠️ Hardware Requirements

### Required Hardware
- **ESP32-S3-Box3** Development Board
  - ESP32-S3 SoC with dual-core 240MHz processor
  - 16MB Flash memory
  - 8MB Octal PSRAM
  - 320x240 IPS LCD display
  - Capacitive touch panel
  - ES8311 audio codec
  - Built-in speaker and microphone
  - USB-C connector for programming and power

### Optional Accessories
- MicroSD card (for future expansion)
- External speaker (via 3.5mm jack)
- USB-C cable for programming

## 💻 Software Requirements

### Required Software
1. **ESP-IDF v5.0 or later**
   - Download from: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/
   - Recommended: ESP-IDF v5.1 or v5.2

2. **Python 3.8 or later**
   - Required for ESP-IDF tools

3. **Git**
   - For cloning repositories

### Supported Operating Systems
- Windows 10/11
- macOS 10.15 or later
- Ubuntu 20.04 or later
- Other Linux distributions

## 📁 Project Structure

```
esp-audio-video/
├── main/                       # Main application code
│   ├── main.c                  # Application entry point
│   ├── app_disp_fs.c           # Display and filesystem logic
│   ├── app_disp_fs.h           # Header file
│   ├── CMakeLists.txt          # Component build configuration
│   └── idf_component.yml       # Component dependencies
├── spiffs_content/             # Files to be stored in SPIFFS
│   ├── Readme.txt              # Sample text file
│   ├── esp_logo.jpg            # Sample image
│   ├── Death Star.jpg          # Sample image
│   ├── Millenium Falcon.jpg    # Sample image
│   └── imperial_march.wav      # Sample audio file
├── doc/                        # Documentation resources
│   └── pic.webp                # Screenshot
├── CMakeLists.txt              # Project build configuration
├── partitions.csv              # Partition table
├── sdkconfig.bsp.esp-box-3     # ESP-Box-3 specific configuration
├── sdkconfig.defaults.esp32s3  # Default ESP32-S3 configuration
└── README.md                   # This file
```

## 🚀 Getting Started

### Installation

#### 1. Install ESP-IDF

Follow the official ESP-IDF installation guide:
https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/

**Quick installation (Linux/macOS):**
```bash
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh esp32s3
. ./export.sh
```

**Windows:**
Use the ESP-IDF Windows Installer from the official website.

#### 2. Clone This Repository

```bash
git clone https://github.com/DanielStuttgart/esp-audio-video.git
cd esp-audio-video
```

### Building the Project

#### 1. Set Up the Environment

Before building, you must set up the ESP-IDF environment in your terminal:

```bash
# Linux/macOS
. $HOME/esp/esp-idf/export.sh

# Windows (Command Prompt)
%userprofile%\esp\esp-idf\export.bat

# Windows (PowerShell)
. $HOME/esp/esp-idf/export.ps1
```

#### 2. Configure the Project

The project is pre-configured for ESP32-S3-Box3, but you can modify settings if needed:

```bash
# Use the pre-configured settings for ESP-Box-3
idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3" reconfigure

# Or open the configuration menu for custom settings
idf.py menuconfig
```

#### 3. Build the Firmware

```bash
idf.py build
```

This will:
- Compile all source code
- Create the SPIFFS filesystem image from `spiffs_content/`
- Generate the final firmware binary

**Expected build time:** 2-5 minutes (depending on your system)

### Flashing the Firmware

#### 1. Connect Your ESP32-S3-Box3

Connect the board to your computer using a USB-C cable.

#### 2. Find the Serial Port

**Linux:**
```bash
ls /dev/ttyUSB* /dev/ttyACM*
# Usually /dev/ttyUSB0 or /dev/ttyACM0
```

**macOS:**
```bash
ls /dev/cu.*
# Usually /dev/cu.usbserial-* or /dev/cu.usbmodem*
```

**Windows:**
- Check Device Manager → Ports (COM & LPT)
- Usually COM3, COM4, etc.

#### 3. Flash the Firmware

```bash
# Replace /dev/ttyUSB0 with your actual port
idf.py -p /dev/ttyUSB0 flash

# Or flash and monitor in one command
idf.py -p /dev/ttyUSB0 flash monitor
```

**Note:** On first flash, you may need to hold the BOOT button on the board.

#### 4. Monitor the Output (Optional)

To see the serial output and debug information:

```bash
idf.py -p /dev/ttyUSB0 monitor
```

Press `Ctrl+]` to exit the monitor.

### Quick Build and Flash

For convenience, you can combine all steps:

```bash
idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3" -p /dev/ttyUSB0 flash monitor
```

## 📖 Usage

### First Boot

After flashing, the application will automatically:
1. Initialize the display and touch panel
2. Mount the SPIFFS filesystem
3. Initialize the audio codec
4. Display the main interface

### User Interface

The application has three main tabs:

#### 1. 📂 Files Tab
- **Browse Files:** Touch any file in the list to open it
- **JPEG Images:** Touch a .jpg file to view it full-screen. Touch the screen to return to the file list
- **Text Files:** Touch a .txt file to read its contents. Scroll to read more if needed
- **Audio Files:** Touch a .wav file to play it. The audio will play through the speaker

#### 2. 🎤 Recording Tab
- **Record Audio:** Press "Start Recording" to record audio from the microphone
- **Stop Recording:** Press "Stop Recording" when finished
- **Playback:** Press "Play Recording" to hear your recorded audio
- **Status:** Watch the status label for recording/playback information

#### 3. ⚙️ Settings Tab
- **Volume Control:** Use the slider to adjust speaker volume (0-100%)
- **Brightness Control:** Use the slider to adjust display brightness (0-100%)
- **Info Display:** View current settings and system information

### Touch Gestures
- **Single Tap:** Select/activate items
- **Swipe:** Scroll through lists (if applicable)
- **Long Press:** (Reserved for future features)

### Adding Your Own Files

To add custom files to the SPIFFS filesystem:

1. Place your files in the `spiffs_content/` directory:
   - JPEG images (.jpg): Max recommended size 800x600
   - WAV audio (.wav): 16-bit PCM, mono or stereo, 22050 Hz recommended
   - Text files (.txt): UTF-8 encoding

2. Rebuild and flash the project:
   ```bash
   idf.py build flash
   ```

**File Size Limits:**
- Total SPIFFS size: ~3MB
- Individual file size: Limited by available SPIFFS space
- Recommended max file size: 500KB for images, 2MB for audio

## ⚙️ Configuration

### Display Configuration

Edit `main/app_disp_fs.h` to change display settings:

```c
/* Default screen brightness (0-100) */
#define APP_DISP_DEFAULT_BRIGHTNESS  (50)
```

### Audio Configuration

Edit `main/app_disp_fs.c` to change audio settings:

```c
#define BUFFER_SIZE     (1024)  // Audio buffer size
#define SAMPLE_RATE     (22050) // Sample rate in Hz
#define DEFAULT_VOLUME  (70)    // Default volume (0-100)
```

### Partition Table

The partition table is defined in `partitions.csv`:

```csv
# Name,   Type, SubType, Offset,  Size
nvs,      data, nvs,     0x9000,  0x6000,
phy_init, data, phy,     0xf000,  0x1000,
factory,  app,  factory, 0x10000, 1M,
storage,  data, spiffs,  0x110000,0x2f0000,
```

**Total storage for files (SPIFFS):** 3,014,656 bytes (~3MB)

### Advanced Configuration

Use `idf.py menuconfig` to access advanced settings:

- **Component config → LVGL configuration:** Customize LVGL settings
- **Component config → ESP32-specific:** Adjust CPU frequency, flash mode
- **Component config → Audio HAL:** Configure audio codec settings

## 🔧 Troubleshooting

### Build Issues

**Problem:** "esp-bsp component not found"
```
Solution: The component will be automatically downloaded via ESP Component Registry.
Make sure you have internet connection during first build.
```

**Problem:** "LVGL configuration errors"
```
Solution: The project uses LVGL9. Make sure CONFIG_LV_CONF_SKIP=y is set in sdkconfig.
Run: idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3" reconfigure
```

### Flash Issues

**Problem:** "Failed to connect to ESP32-S3"
```
Solution: 
1. Hold the BOOT button while connecting USB
2. Try a different USB cable
3. Check if drivers are installed (Windows: install USB serial drivers)
```

**Problem:** "Not enough space in flash"
```
Solution: 
1. Remove some files from spiffs_content/
2. Or modify partitions.csv to allocate more space to SPIFFS
```

### Runtime Issues

**Problem:** "Display shows garbage or doesn't initialize"
```
Solution: 
1. Power cycle the board (disconnect and reconnect USB)
2. Ensure you're using the correct sdkconfig for ESP-Box-3
3. Re-flash the firmware
```

**Problem:** "No audio output"
```
Solution: 
1. Check volume setting in Settings tab
2. Ensure WAV file is correct format (16-bit PCM)
3. Try increasing volume to 100%
```

**Problem:** "Touch not responding"
```
Solution: 
1. Clean the screen surface
2. Ensure touch panel is properly connected (hardware issue)
3. Re-calibrate if needed (check BSP documentation)
```

**Problem:** "Files not showing in file list"
```
Solution: 
1. Ensure files are in spiffs_content/ before building
2. Rebuild the project completely: idf.py fullclean build flash
3. Check file names don't have special characters
```

### Debugging

Enable verbose logging:

```bash
# Monitor with detailed logs
idf.py -p /dev/ttyUSB0 monitor

# In code, increase log level in main.c:
esp_log_level_set("*", ESP_LOG_DEBUG);
```

## 🔬 Technical Details

### Hardware Specifications

**ESP32-S3-Box3 Board:**
- **MCU:** ESP32-S3 (Xtensa dual-core 32-bit LX7, up to 240MHz)
- **Flash:** 16MB QSPI flash
- **RAM:** 512KB SRAM + 8MB PSRAM (Octal)
- **Display:** 320x240 IPS LCD, 16-bit color (RGB565)
- **Touch:** Capacitive touch panel (GT911)
- **Audio Codec:** ES8311
- **Speaker:** 2W, 8Ω
- **Microphone:** MEMS digital microphone
- **Connectivity:** Wi-Fi 802.11 b/g/n, Bluetooth 5.0 (LE)

### Software Architecture

```
┌─────────────────────────────────────┐
│         Application Layer           │
│  (main.c, app_disp_fs.c)           │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│         LVGL Graphics Library       │
│      (UI rendering, touch input)    │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│    ESP-BSP (Board Support Package)  │
│  (Display, Audio, I2C, SPIFFS init) │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│           ESP-IDF (FreeRTOS)        │
│   (Drivers, OS, Networking, etc.)   │
└─────────────────────────────────────┘
```

### Memory Usage

**Typical Runtime Memory:**
- DRAM: ~120KB
- IRAM: ~80KB  
- PSRAM: Used for display framebuffer (~150KB) and LVGL objects

**Flash Usage:**
- Application: ~1MB
- SPIFFS: ~3MB (for user files)

### Performance

- **Display Refresh Rate:** 30-60 FPS (depending on content)
- **Touch Response Time:** <50ms
- **JPEG Decode Time:** ~100-500ms (depending on image size)
- **Audio Playback:** Real-time, no lag

### Key Dependencies

- **ESP-IDF:** v5.0+
- **ESP-BSP:** Latest (auto-downloaded)
- **LVGL:** v9.x (configured via BSP)
- **esp_jpeg:** Latest (for JPEG decoding)

## 🤝 Contributing

Contributions are welcome! Please follow these guidelines:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

### Development Guidelines

- Follow ESP-IDF coding style
- Add comments for complex logic
- Test on actual hardware before submitting
- Update documentation if adding new features

## 📄 License

This project is based on ESP-IDF examples which are provided under the Apache License 2.0.

```
SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
SPDX-License-Identifier: Apache-2.0
```

Main application code (main.c) is provided under CC0-1.0 (public domain).

See individual files for specific license information.

## 📚 Additional Resources

### Official Documentation
- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [ESP-BSP Documentation](https://github.com/espressif/esp-bsp)
- [LVGL Documentation](https://docs.lvgl.io/)

### Community Support
- [ESP32 Forum](https://www.esp32.com/)
- [ESP-IDF GitHub Issues](https://github.com/espressif/esp-idf/issues)
- [Reddit r/esp32](https://www.reddit.com/r/esp32/)

### Related Examples
- [ESP-BSP Examples](https://github.com/espressif/esp-bsp/tree/master/examples)
- [ESP-IDF Examples](https://github.com/espressif/esp-idf/tree/master/examples)

---

**Made with ❤️ for the ESP32-S3-Box3 community**

For questions or issues, please open an issue on GitHub.