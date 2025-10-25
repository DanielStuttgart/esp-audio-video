# Quick Reference Card

Fast reference for common tasks and commands.

## 🚀 Quick Commands

### Build & Flash
```bash
# First time build
idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3" build

# Build only
idf.py build

# Flash only
idf.py -p /dev/ttyUSB0 flash

# Build, flash, and monitor
idf.py -p /dev/ttyUSB0 flash monitor

# Clean build
idf.py fullclean build
```

### Troubleshooting
```bash
# Erase everything and reflash
idf.py -p /dev/ttyUSB0 erase-flash flash

# Reconfigure from scratch
idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3" reconfigure

# Monitor serial output
idf.py -p /dev/ttyUSB0 monitor
```

### Environment Setup
```bash
# Linux/macOS
. $HOME/esp/esp-idf/export.sh

# Windows CMD
%userprofile%\esp\esp-idf\export.bat

# Windows PowerShell
. $HOME/esp/esp-idf/export.ps1
```

## 📁 Project Structure

```
esp-audio-video/
├── main/              # Source code
├── spiffs_content/    # Files for SPIFFS
├── doc/               # Documentation images
├── CMakeLists.txt     # Build config
└── *.md               # Documentation
```

## 🔧 Configuration Files

| File | Purpose |
|------|---------|
| `sdkconfig.bsp.esp-box-3` | Board-specific config |
| `partitions.csv` | Memory partitions |
| `main/idf_component.yml` | Dependencies |

## 🎛️ Key Settings

### Brightness (0-100%)
```c
// main/app_disp_fs.h
#define APP_DISP_DEFAULT_BRIGHTNESS  (50)
```

### Volume (0-100%)
```c
// main/app_disp_fs.c
#define DEFAULT_VOLUME  (70)
```

### Audio Settings
```c
// main/app_disp_fs.c
#define BUFFER_SIZE     (1024)   // Buffer size
#define SAMPLE_RATE     (22050)  // Hz
#define RECORDING_LENGTH (160)   // Buffers
```

## 📂 File Specifications

### JPEG Images
- Format: Baseline JPEG, RGB
- Max size: 800x600 recommended
- Max file: 500KB recommended

### WAV Audio
- Format: PCM uncompressed
- Bits: 16-bit
- Channels: Mono or Stereo
- Sample rate: 22050 Hz (recommended)

### Text Files
- Encoding: UTF-8
- Line breaks: \n or \r\n

## 🐛 Common Issues

### Build Fails
```bash
idf.py fullclean
idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3" reconfigure build
```

### Flash Fails
1. Hold BOOT button
2. Try: `idf.py -p PORT -b 115200 flash`
3. Try different USB cable

### No Display
```bash
idf.py -p /dev/ttyUSB0 erase-flash flash
```

### No Files
```bash
# Rebuild SPIFFS image
idf.py fullclean build flash
```

## 📍 Serial Ports

| OS | Common Ports |
|----|--------------|
| Linux | `/dev/ttyUSB0`, `/dev/ttyACM0` |
| macOS | `/dev/cu.usbserial-*` |
| Windows | `COM3`, `COM4`, `COM5` |

## 🎯 UI Navigation

### Tabs
- **Files:** Browse and open files
- **Recording:** Record/playback audio
- **Settings:** Adjust volume/brightness

### Touch Actions
- **Tap:** Select/activate
- **Swipe:** Scroll (if applicable)

## 📊 Memory Layout

| Partition | Size | Use |
|-----------|------|-----|
| Factory | 1 MB | Application firmware |
| SPIFFS | ~3 MB | User files |
| NVS | 24 KB | Settings storage |
| PHY Init | 4 KB | WiFi/BT calibration |

## 🔍 Debugging

### Enable Debug Logs
```c
// In main.c
esp_log_level_set("*", ESP_LOG_DEBUG);
```

### Check Memory
```c
ESP_LOGI(TAG, "Free: %d", esp_get_free_heap_size());
```

### Monitor Performance
- LVGL performance monitor enabled by default
- Shows FPS on screen

## 📚 Documentation

| File | When to Use |
|------|-------------|
| **QUICK_START.md** | First time setup |
| **README.md** | Complete guide |
| **TROUBLESHOOTING.md** | Problems |
| **FAQ.md** | Questions |
| **API.md** | Technical details |
| **CONTRIBUTING.md** | Want to contribute |

## 🔗 Useful Links

- [Repository](https://github.com/DanielStuttgart/esp-audio-video)
- [ESP-IDF Docs](https://docs.espressif.com/projects/esp-idf/)
- [ESP32 Forum](https://www.esp32.com/)
- [LVGL Docs](https://docs.lvgl.io/)

## 💡 Tips

1. Always source ESP-IDF environment first
2. Use `-D SDKCONFIG_DEFAULTS=...` for clean builds
3. Check serial monitor for errors
4. Keep images small (<500KB)
5. Use 22050 Hz for audio
6. Back up your changes regularly

---

**Need more help?** See full documentation in README.md
