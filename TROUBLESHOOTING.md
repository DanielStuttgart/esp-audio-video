# Troubleshooting Guide

Detailed troubleshooting steps for common issues with the ESP32-S3-Box3 Display Audio Photo application.

## Table of Contents

- [Build Issues](#build-issues)
- [Flash Issues](#flash-issues)
- [Display Issues](#display-issues)
- [Touch Issues](#touch-issues)
- [Audio Issues](#audio-issues)
- [File System Issues](#file-system-issues)
- [Performance Issues](#performance-issues)
- [Advanced Debugging](#advanced-debugging)

## Build Issues

### Error: "esp-box-3 component not found"

**Symptom:**
```
CMake Error at main/idf_component.yml:
  Could not find component esp-box-3
```

**Solution:**
1. Ensure you have internet connection (component is downloaded from ESP Component Registry)
2. Clear component cache and rebuild:
   ```bash
   rm -rf managed_components/
   rm -f dependencies.lock
   idf.py reconfigure
   idf.py build
   ```
3. If still failing, manually specify component manager mirror:
   ```bash
   export IDF_COMPONENT_REGISTRY_URL=https://components.espressif.com
   idf.py build
   ```

### Error: "LVGL configuration errors"

**Symptom:**
```
error: LVGL config error: CONFIG_LV_CONF_SKIP must be enabled
```

**Solution:**
1. Ensure you're using the correct sdkconfig:
   ```bash
   idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3" reconfigure
   ```
2. Clean and rebuild:
   ```bash
   idf.py fullclean
   idf.py build
   ```
3. Verify sdkconfig.bsp.esp-box-3 contains:
   ```
   CONFIG_LV_CONF_SKIP=y
   ```

### Error: "ninja: build stopped: subcommand failed"

**Symptom:**
Build fails with compilation errors in source files.

**Solutions:**

**For undefined reference errors:**
```bash
# Clean build directory
idf.py fullclean
# Rebuild from scratch
idf.py build
```

**For missing header files:**
```bash
# Update component registry
idf.py reconfigure
```

**For syntax errors in your code:**
- Check the error message for the file and line number
- Review recent changes
- Compare with working example code

### Error: "Toolchain not found"

**Symptom:**
```
CMake Error: Could not find toolchain file
```

**Solution:**
1. Set up ESP-IDF environment:
   ```bash
   . $HOME/esp/esp-idf/export.sh
   ```
2. Verify toolchain installation:
   ```bash
   xtensa-esp32s3-elf-gcc --version
   ```
3. If missing, reinstall ESP-IDF tools:
   ```bash
   cd $IDF_PATH
   ./install.sh esp32s3
   ```

### Error: "SPIFFS image creation failed"

**Symptom:**
```
Error creating SPIFFS image: file too large
```

**Solution:**
1. Check total size of files in spiffs_content/:
   ```bash
   du -sh spiffs_content/
   ```
2. SPIFFS partition is ~3MB. Remove or compress large files:
   ```bash
   # Remove large audio file
   rm spiffs_content/imperial_march.wav
   # Or compress images
   # Use online tools or imagemagick
   ```
3. Or increase SPIFFS partition size in partitions.csv:
   ```csv
   storage,  data, spiffs,  0x110000,0x3f0000,
   ```
   Note: This reduces available flash for firmware updates

## Flash Issues

### Error: "Serial port not found"

**Symptom:**
```
serial.serialutil.SerialException: could not open port /dev/ttyUSB0
```

**Solutions:**

**Linux:**
1. Check if device is detected:
   ```bash
   ls -l /dev/ttyUSB* /dev/ttyACM*
   dmesg | tail -20
   ```
2. Add user to dialout group:
   ```bash
   sudo usermod -a -G dialout $USER
   # Log out and back in for changes to take effect
   ```
3. Install/update drivers:
   ```bash
   # For CH340/CH341 USB-to-serial
   sudo apt-get install linux-modules-extra-$(uname -r)
   ```

**macOS:**
1. Check for device:
   ```bash
   ls -l /dev/cu.*
   ```
2. Install drivers if needed:
   - Download from Silicon Labs (CP210x) or WCH (CH340)

**Windows:**
1. Check Device Manager → Ports (COM & LPT)
2. Install drivers:
   - Silicon Labs CP210x driver
   - Or WCH CH340 driver
3. Try different USB port

### Error: "Failed to connect to ESP32-S3"

**Symptom:**
```
A fatal error occurred: Failed to connect to ESP32-S3: Wrong boot mode detected
```

**Solutions:**

**Method 1: Boot button method**
1. Hold BOOT button on board
2. Press and release RESET button
3. Release BOOT button
4. Run flash command immediately:
   ```bash
   idf.py -p /dev/ttyUSB0 flash
   ```

**Method 2: Automatic bootloader**
```bash
# Some boards need extra time
idf.py -p /dev/ttyUSB0 flash --before default_reset --after hard_reset
```

**Method 3: Manual bootloader mode**
```bash
# Put into download mode manually
esptool.py -p /dev/ttyUSB0 -b 115200 --before default_reset --after hard_reset chip_id
# Then flash
idf.py -p /dev/ttyUSB0 flash
```

**Method 4: USB cable issue**
- Try a different USB cable (must be data cable, not charge-only)
- Try different USB port on computer
- Use USB 2.0 port instead of USB 3.0

### Error: "Partition table does not fit in configured flash size"

**Symptom:**
```
Error: Partition table does not fit in configured flash size
```

**Solution:**
1. Verify flash size in sdkconfig:
   ```bash
   grep FLASHSIZE sdkconfig.bsp.esp-box-3
   # Should show: CONFIG_ESPTOOLPY_FLASHSIZE_16MB=y
   ```
2. Reconfigure if needed:
   ```bash
   idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3" reconfigure
   ```

### Error: "Flash write error" or "Flash read error"

**Symptom:**
```
A fatal error occurred: Timed out waiting for packet content
```

**Solutions:**
1. Lower baud rate:
   ```bash
   idf.py -p /dev/ttyUSB0 -b 115200 flash
   ```
2. Erase flash first:
   ```bash
   idf.py -p /dev/ttyUSB0 erase-flash
   idf.py -p /dev/ttyUSB0 flash
   ```
3. Check power supply:
   - Use powered USB hub if using laptop
   - Connect directly to computer USB port (not through hub)

## Display Issues

### Issue: Display shows nothing (black screen)

**Possible Causes & Solutions:**

**1. Backlight not initialized:**
```c
// Check main.c has this line:
bsp_display_brightness_set(APP_DISP_DEFAULT_BRIGHTNESS);
```

**2. Display not started:**
```bash
# Check serial monitor for errors:
idf.py -p /dev/ttyUSB0 monitor
# Look for "Starting display..." message
```

**3. Hardware issue:**
- Check display ribbon cable connection
- Try power cycling (unplug USB, wait 5 seconds, replug)

### Issue: Display shows garbage/corrupted graphics

**Solutions:**

**1. PSRAM not enabled:**
```bash
# Verify in sdkconfig:
grep SPIRAM sdkconfig.bsp.esp-box-3
# Should have:
# CONFIG_SPIRAM=y
# CONFIG_SPIRAM_MODE_OCT=y
```

**2. Wrong display configuration:**
- Ensure using sdkconfig.bsp.esp-box-3 (not other board configs)

**3. LVGL buffer issue:**
```bash
# Rebuild with clean state:
idf.py fullclean
idf.py -D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3" build flash
```

### Issue: Display freezes

**Debugging Steps:**

1. Check serial monitor for errors:
   ```bash
   idf.py monitor
   ```

2. Check for memory issues:
   ```c
   // Add to main.c after initialization:
   ESP_LOGI(TAG, "Free heap: %d bytes", esp_get_free_heap_size());
   ```

3. Disable watchdog temporarily (for testing only):
   ```bash
   idf.py menuconfig
   # Component config → ESP System Settings → 
   # Interrupt watchdog timeout → Increase to 1000ms
   ```

4. Check task stack:
   ```bash
   # In menuconfig:
   # Component config → Common ESP-related → 
   # Main task stack size → Increase to 8192
   ```

### Issue: Slow/choppy display

**Performance Optimization:**

1. Enable PSRAM:
   - Should already be enabled in sdkconfig.bsp.esp-box-3

2. Increase CPU frequency:
   ```bash
   # Should be 240MHz in sdkconfig.bsp.esp-box-3:
   CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_240=y
   ```

3. Check LVGL performance monitor:
   - Enabled by default in sdkconfig.bsp.esp-box-3
   - Shows FPS on screen
   - Target: 30+ FPS

## Touch Issues

### Issue: Touch not responding

**Solutions:**

1. **Check I2C initialization:**
   ```bash
   # Monitor output should show:
   # "Initializing I2C..."
   idf.py monitor
   ```

2. **Clean screen:**
   - Wipe display with microfiber cloth
   - Remove any screen protector

3. **Calibrate touch (if supported):**
   ```bash
   # Check BSP documentation for calibration
   # Usually automatic on ESP32-S3-Box3
   ```

4. **Check touch controller:**
   ```bash
   # Add debug logging in main.c:
   esp_log_level_set("touch", ESP_LOG_DEBUG);
   ```

5. **Hardware check:**
   - Ensure touch panel ribbon cable is connected
   - Try reseating the cable

### Issue: Touch inaccurate or drifting

**Solutions:**

1. **Ground the board:**
   - Ensure USB cable is properly connected
   - Try different USB port/cable

2. **Check for electrical interference:**
   - Move away from other electronic devices
   - Don't touch metal parts while using

3. **Update touch configuration:**
   - Check ESP-BSP repository for latest touch driver updates

## Audio Issues

### Issue: No audio output

**Troubleshooting Steps:**

1. **Check volume:**
   - Go to Settings tab
   - Ensure volume slider is not at 0
   - Try setting to 100%

2. **Check audio initialization:**
   ```bash
   # Monitor should show:
   # "Initializing audio..."
   # "ES8311 in Slave mode..."
   idf.py monitor
   ```

3. **Verify I2S configuration:**
   - Should be auto-configured by BSP
   - Check for I2S errors in monitor output

4. **Test with different file:**
   - Try playing different WAV file
   - Ensure file is valid WAV format

5. **Check codec settings:**
   ```c
   // In app_disp_fs.c, verify:
   #define DEFAULT_VOLUME  (70)
   ```

### Issue: Audio quality poor/distorted

**Solutions:**

1. **Check WAV file format:**
   - Must be PCM uncompressed
   - 16-bit samples
   - Sample rate: 22050 Hz (or 44100 Hz)
   - Use online converter if needed

2. **Check file integrity:**
   ```bash
   # On computer, verify WAV file plays correctly
   ffmpeg -i yourfile.wav
   ```

3. **Reduce volume:**
   - High volume can cause distortion
   - Try 50-70% volume

4. **Check buffer size:**
   ```c
   // In app_disp_fs.c:
   #define BUFFER_SIZE (1024)
   // Increase if choppy audio
   ```

### Issue: Audio playback stutters

**Solutions:**

1. **SPIFFS read speed:**
   - Large files may cause stuttering
   - Keep audio files under 2MB

2. **Increase audio buffer:**
   ```c
   // In app_disp_fs.c, increase:
   #define BUFFER_SIZE (2048)
   // Rebuild and flash
   ```

3. **CPU priority:**
   - Close monitoring during playback
   - Ensure CPU not overloaded

### Issue: Microphone not recording

**Solutions:**

1. **Check microphone support:**
   - ESP32-S3-Box3 has built-in microphone
   - Verify BSP supports recording

2. **Check recording code:**
   - Recording tab should have Start/Stop buttons
   - Check monitor for recording messages

3. **Permissions:**
   - No permissions needed on ESP32
   - Check if recording file can be created

4. **Storage space:**
   - Ensure SPIFFS has space for recording
   - Recording is ~160KB

## File System Issues

### Issue: Files not appearing in file list

**Solutions:**

1. **Verify SPIFFS mounted:**
   ```bash
   # Monitor should show:
   # "Mounting SPIFFS..."
   idf.py monitor
   ```

2. **Rebuild SPIFFS image:**
   ```bash
   # Ensure files are in spiffs_content/
   ls -la spiffs_content/
   # Rebuild completely
   idf.py fullclean build flash
   ```

3. **Check file names:**
   - No special characters
   - Valid extensions (.jpg, .wav, .txt)

4. **Verify partition:**
   ```bash
   # Check partitions.csv:
   storage,  data, spiffs,  0x110000,0x2f0000,
   ```

### Issue: "Failed to mount SPIFFS" error

**Solutions:**

1. **Format SPIFFS:**
   ```bash
   idf.py erase-flash
   idf.py flash
   ```

2. **Check partition table:**
   - Ensure partitions.csv is correct
   - Rebuild with:
     ```bash
     idf.py fullclean build flash
     ```

3. **SPIFFS corrupted:**
   ```bash
   # Erase and reflash
   idf.py -p /dev/ttyUSB0 erase-flash flash
   ```

### Issue: Cannot open specific file

**Debugging:**

1. **Check file exists:**
   ```bash
   # Add debug logging:
   DIR *dir = opendir("/spiffs");
   if (dir) {
       struct dirent *de;
       while ((de = readdir(dir)) != NULL) {
           ESP_LOGI(TAG, "File: %s", de->d_name);
       }
       closedir(dir);
   }
   ```

2. **Check file size:**
   - Files might be too large
   - JPEG decoder has memory limits

3. **Check file format:**
   - JPEG: Baseline, RGB
   - WAV: PCM, 16-bit
   - TXT: UTF-8

## Performance Issues

### Issue: UI lags or slow response

**Optimization Tips:**

1. **Enable all optimizations:**
   - Should be set in sdkconfig.bsp.esp-box-3:
     ```
     CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ_240=y
     CONFIG_SPIRAM=y
     CONFIG_SPIRAM_SPEED_80M=y
     ```

2. **Check memory usage:**
   ```c
   ESP_LOGI(TAG, "Free heap: %d", esp_get_free_heap_size());
   ESP_LOGI(TAG, "Minimum free heap: %d", esp_get_minimum_free_heap_size());
   ```

3. **Reduce complexity:**
   - Simplify LVGL objects
   - Use smaller images
   - Reduce animation

4. **Monitor task CPU:**
   ```c
   vTaskGetRunTimeStats(pBuffer);
   ESP_LOGI(TAG, "Task stats:\n%s", pBuffer);
   ```

## Advanced Debugging

### Enable All Debug Logs

```c
// In main.c, add after includes:
void app_main(void)
{
    // Set all logs to DEBUG level
    esp_log_level_set("*", ESP_LOG_DEBUG);
    
    // Or specific components:
    esp_log_level_set("DISP", ESP_LOG_DEBUG);
    esp_log_level_set("ESP-BOX", ESP_LOG_DEBUG);
    esp_log_level_set("lvgl", ESP_LOG_DEBUG);
    
    // Rest of initialization...
}
```

### Use GDB Debugger

```bash
# Build with debug symbols:
idf.py -D CMAKE_BUILD_TYPE=Debug build

# Flash and start GDB:
idf.py -p /dev/ttyUSB0 flash monitor gdb
```

### Monitor Memory Leaks

```c
// Add to periodic task:
void memory_monitor_task(void *pvParameters)
{
    while (1) {
        ESP_LOGI(TAG, "Free heap: %d bytes", esp_get_free_heap_size());
        ESP_LOGI(TAG, "Min free heap: %d bytes", esp_get_minimum_free_heap_size());
        ESP_LOGI(TAG, "Largest free block: %d bytes", heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT));
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
```

### Core Dump Analysis

Enable core dumps in menuconfig:
```bash
idf.py menuconfig
# Component config → ESP System Settings → 
# Core dump destination → Flash
```

After crash:
```bash
idf.py coredump-info
idf.py coredump-debug
```

### JTAG Debugging

For advanced debugging with JTAG:
1. Connect JTAG debugger (ESP-Prog or similar)
2. Configure OpenOCD
3. Use Eclipse or VS Code with debug extension

## Getting Help

If you still have issues:

1. **Check GitHub Issues:**
   - https://github.com/DanielStuttgart/esp-audio-video/issues

2. **ESP32 Forum:**
   - https://www.esp32.com/
   - Search for similar issues

3. **ESP-IDF Documentation:**
   - https://docs.espressif.com/projects/esp-idf/

4. **ESP-BSP Repository:**
   - https://github.com/espressif/esp-bsp/issues

5. **Create New Issue:**
   - Provide full error log
   - Include ESP-IDF version
   - Describe steps to reproduce
   - Include hardware details

## Hardware Issues

If you suspect hardware problems:

- **Contact supplier** for replacement/repair
- **Check warranty** on ESP32-S3-Box3
- **Test with different board** if available
- **Visual inspection** for damage

---

**Remember:** Most issues are solved by:
1. Clean rebuild: `idf.py fullclean build flash`
2. Erase flash: `idf.py erase-flash flash`
3. Using correct sdkconfig: `-D SDKCONFIG_DEFAULTS="sdkconfig.bsp.esp-box-3"`
4. Checking serial monitor for errors
