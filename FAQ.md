# Frequently Asked Questions (FAQ)

Common questions and answers about the ESP32-S3-Box3 Display Audio Photo project.

## General Questions

### What is this project?

This is a comprehensive multimedia application for the ESP32-S3-Box3 development board that demonstrates:
- Display and touch capabilities
- Audio playback and recording
- File system management
- JPEG image viewing
- Text file reading

It's based on the [ESP-BSP display_audio_photo example](https://github.com/espressif/esp-bsp/tree/master/examples/display_audio_photo).

### What hardware do I need?

You need:
- **ESP32-S3-Box3** development board (required)
- **USB-C cable** for programming and power
- A computer running Windows, macOS, or Linux

That's it! The board has everything built-in (display, touch, speaker, microphone).

### Can I use this with other ESP32 boards?

No, this project is specifically configured for the ESP32-S3-Box3. However, the code can be adapted for other boards with displays. You would need to:
- Change the BSP dependency in `main/idf_component.yml`
- Update the sdkconfig files
- Adjust partition table if needed

### Is this project free to use?

Yes! The code is released under:
- **CC0-1.0 (Public Domain)** for main application code
- **Apache 2.0** for supporting code

See [LICENSE](LICENSE) for details.

## Installation & Setup

### Do I need to install ESP-IDF?

Yes, ESP-IDF is required to build and flash the project. It's the official development framework for ESP32 devices.

Follow the [installation guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/).

### Which ESP-IDF version should I use?

**Recommended:** ESP-IDF v5.1 or v5.2

**Minimum:** ESP-IDF v5.0

The project may work with newer versions, but has been tested with v5.1.

### How long does the first build take?

First build typically takes **2-5 minutes** depending on your computer. Subsequent builds are much faster (30 seconds to 2 minutes).

### Can I use VSCode or other IDE?

Yes! ESP-IDF supports:
- **VSCode** with ESP-IDF extension (recommended)
- **Eclipse** with ESP-IDF plugin
- **CLion** with ESP-IDF support
- **Command line** (terminal)

See [VSCode setup guide](https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/install.md).

### Where can I buy an ESP32-S3-Box3?

Available from:
- Espressif official distributors
- Amazon
- AliExpress
- Mouser
- DigiKey
- Local electronics suppliers

Search for "ESP32-S3-Box3" or "ESP-BOX-3".

## Features & Functionality

### What file types are supported?

| Type | Extensions | Format Requirements |
|------|-----------|-------------------|
| Images | .jpg, .jpeg | Baseline JPEG, RGB |
| Audio | .wav | PCM, 16-bit, Mono/Stereo |
| Text | .txt | UTF-8 encoding |

### Can I add MP3 support?

Not by default, but you can add MP3 decoding:
1. Add an MP3 decoder library (e.g., libhelix)
2. Modify file type detection
3. Add MP3 playback handler

This requires additional work and increases binary size.

### How much storage is available for files?

**SPIFFS partition: ~3MB** (3,014,656 bytes)

This is shared among all your files (images, audio, text).

### Can I use an SD card for more storage?

The ESP32-S3-Box3 doesn't have a built-in SD card slot, but you could:
- Add external SD card module via SPI
- Modify the code to support SD card filesystem
- This requires hardware modification and code changes

### What's the maximum image size?

**Technical limit:** Depends on available memory  
**Recommended:** 800x600 pixels or smaller  
**File size:** Keep under 500KB for best performance

Larger images work but take longer to decode.

### What audio sample rates are supported?

Common rates work:
- 8000 Hz ✓
- 11025 Hz ✓
- 16000 Hz ✓
- **22050 Hz** ✓ (default, recommended)
- 44100 Hz ✓
- 48000 Hz ✓

Lower sample rates = smaller files  
Higher sample rates = better quality

### How long can I record audio?

Default: **~3.7 seconds** (configurable)

Calculation:
```
RECORDING_LENGTH (160) × BUFFER_SIZE (1024 bytes)
= 163,840 bytes
÷ (22050 Hz × 16 bits ÷ 8 bits/byte)
= ~3.72 seconds
```

To record longer, increase `RECORDING_LENGTH` in `app_disp_fs.c`.

### Can I record and save multiple audio files?

Currently, only one recording is kept (`recording.wav`), which is overwritten on each new recording. To support multiple recordings, you would need to:
1. Implement file naming system
2. Add UI for managing recordings
3. Manage SPIFFS storage space

## Development & Customization

### How do I add my own files?

1. Place files in `spiffs_content/` directory
2. Rebuild the project:
   ```bash
   idf.py build flash
   ```
3. Files will be embedded in firmware

### Can I change the UI colors/theme?

Yes! Modify LVGL theme in the code:

```c
// In app_disp_fs.c, after LVGL init:
lv_theme_t * theme = lv_theme_default_init(
    lv_disp_get_default(),
    lv_color_hex(0x00FF00),  // Primary color
    lv_color_hex(0xFF0000),  // Secondary color
    true,  // Dark mode
    &lv_font_montserrat_14
);
```

### How do I change default volume/brightness?

Edit `main/app_disp_fs.h`:

```c
#define APP_DISP_DEFAULT_BRIGHTNESS  (50)  // Change 0-100
```

Edit `main/app_disp_fs.c`:

```c
#define DEFAULT_VOLUME  (70)  // Change 0-100
```

### Can I add Wi-Fi functionality?

Yes! The ESP32-S3 has Wi-Fi. You can add:
- Web server to upload files
- OTA (Over-The-Air) updates
- IoT connectivity
- Network audio streaming

This requires adding Wi-Fi initialization code and networking features.

### How do I add a new tab to the UI?

1. In `app_disp_lvgl_show()`, increase tab count
2. Create tab content function
3. Add tab to tabview:
   ```c
   lv_obj_t *tab_new = lv_tabview_add_tab(tabview, "My Tab");
   // Add your content to tab_new
   ```

### Can I use a different display?

The code is tied to the ESP32-S3-Box3 BSP. For different displays:
- Use appropriate BSP component
- Or configure display driver manually
- Update `idf_component.yml`

## Technical Questions

### What's the display resolution?

**320 × 240 pixels** (QVGA)  
**Color depth:** 16-bit (RGB565)  
**Type:** IPS LCD

### What's the CPU speed?

**Default: 240 MHz** (dual-core)

You can change this in menuconfig, but 240 MHz is recommended for smooth graphics.

### How much RAM is available?

- **SRAM:** 512 KB
- **PSRAM:** 8 MB (Octal)

PSRAM is used for display buffers and large allocations.

### What's the flash size?

**16 MB** total, partitioned as:
- Factory app: 1 MB
- SPIFFS: ~3 MB
- NVS: 24 KB
- PHY init: 4 KB
- Remaining: ~12 MB (unused)

### Does it support Bluetooth?

The ESP32-S3 has Bluetooth 5.0 (LE), but it's not used in this project. You can add Bluetooth functionality if needed.

### Can I use this for commercial projects?

Yes! The code license (CC0-1.0 and Apache 2.0) allows commercial use. However:
- Replace sample content (images/audio) with your own
- Review licenses of dependencies
- Follow license requirements (e.g., attribution for Apache 2.0 code)

## Performance & Optimization

### What frame rate does the display achieve?

**Typical: 30-60 FPS** depending on content complexity

Use LVGL performance monitor (enabled by default) to see actual FPS.

### Why is JPEG decoding slow?

JPEG decoding uses hardware acceleration, but:
- Large images take longer
- Complex images take longer
- Typical time: 100-500ms

This is normal for embedded devices.

### How can I improve performance?

1. **Keep images small** (max 800x600)
2. **Simplify UI** (fewer LVGL objects)
3. **Optimize code** (reduce loops, use efficient algorithms)
4. **Enable compiler optimizations** (already enabled)
5. **Use PSRAM effectively** (already configured)

### Does it support video?

No built-in video support. Adding video would require:
- Video decoder library (e.g., MJPEG)
- Significant memory and CPU resources
- Frame-by-frame playback

Simple animations are possible with image sequences.

## Troubleshooting

### Build fails - what should I do?

Most common fixes:
```bash
# Clean and rebuild
idf.py fullclean
idf.py build

# Or with explicit sdkconfig
idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3" reconfigure build
```

See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for detailed help.

### Flash fails - help!

1. **Hold BOOT button** on board while connecting USB
2. **Try different USB cable** (must be data cable)
3. **Lower baud rate:** `idf.py -p PORT -b 115200 flash`
4. **Try different USB port**

### Display doesn't work - black screen!

1. **Check backlight:** Should be set in code
2. **Power cycle:** Unplug and replug USB
3. **Re-flash firmware:** `idf.py erase-flash flash`

### No files showing in file list!

1. **Check files exist:** `ls spiffs_content/`
2. **Rebuild SPIFFS:** `idf.py fullclean build flash`
3. **Check file names:** No special characters

### Touch doesn't work!

1. **Clean screen** with microfiber cloth
2. **Check initialization** in serial monitor
3. **Re-flash firmware**
4. **Check hardware** (ribbon cable)

## Contributing

### How can I contribute?

We welcome contributions! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

Ways to help:
- Fix bugs
- Add features
- Improve documentation
- Report issues
- Share your projects

### I found a bug - what should I do?

1. **Check if already reported** in [GitHub Issues](https://github.com/DanielStuttgart/esp-audio-video/issues)
2. **Create new issue** with:
   - Clear description
   - Steps to reproduce
   - Expected vs actual behavior
   - Serial monitor output
   - ESP-IDF version

### Can I use this code in my project?

Yes! The code is open source. Please:
- Follow license requirements
- Give attribution if required by license
- Share improvements back to community (optional but appreciated)

## Resources

### Where can I learn more about ESP32-S3?

- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [ESP32 Forum](https://www.esp32.com/)

### Where can I learn LVGL?

- [LVGL Documentation](https://docs.lvgl.io/)
- [LVGL Examples](https://docs.lvgl.io/master/examples.html)
- [LVGL Forum](https://forum.lvgl.io/)

### Where can I get help?

1. **Read documentation:**
   - [README.md](README.md)
   - [TROUBLESHOOTING.md](TROUBLESHOOTING.md)
   - [API.md](API.md)

2. **Search GitHub Issues:**
   - [Project Issues](https://github.com/DanielStuttgart/esp-audio-video/issues)

3. **Ask community:**
   - [ESP32 Forum](https://www.esp32.com/)
   - [Reddit r/esp32](https://www.reddit.com/r/esp32/)

4. **Create issue:**
   - [New Issue](https://github.com/DanielStuttgart/esp-audio-video/issues/new)

## Still Have Questions?

**Didn't find your answer?**

- 📖 Check [README.md](README.md) for detailed information
- 🔧 See [TROUBLESHOOTING.md](TROUBLESHOOTING.md) for problem solving
- 💻 Review [API.md](API.md) for technical details
- 🐛 [Open an issue](https://github.com/DanielStuttgart/esp-audio-video/issues/new) on GitHub
- 💬 Ask on [ESP32 Forum](https://www.esp32.com/)

---

**Happy Making! 🚀**
