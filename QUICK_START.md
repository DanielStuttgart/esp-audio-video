# Quick Start Guide

Get your ESP32-S3-Box3 Display Audio Photo example running in 5 minutes!

## Prerequisites Checklist

- [ ] ESP32-S3-Box3 board
- [ ] USB-C cable
- [ ] Computer (Windows, macOS, or Linux)
- [ ] ESP-IDF installed (v5.0 or later)

## Step-by-Step Instructions

### 1. Install ESP-IDF (First Time Only)

**Linux/macOS:**
```bash
mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh esp32s3
```

**Windows:**
Download and run the [ESP-IDF Windows Installer](https://dl.espressif.com/dl/esp-idf/)

### 2. Clone This Repository

```bash
git clone https://github.com/DanielStuttgart/esp-audio-video.git
cd esp-audio-video
```

### 3. Set Up Environment

Open a new terminal and run:

**Linux/macOS:**
```bash
. $HOME/esp/esp-idf/export.sh
```

**Windows Command Prompt:**
```cmd
%userprofile%\esp\esp-idf\export.bat
```

**Windows PowerShell:**
```powershell
. $HOME/esp/esp-idf/export.ps1
```

### 4. Build the Project

```bash
idf.py build
```

⏱️ **First build takes 2-5 minutes** (subsequent builds are faster)

### 5. Connect Your Board

1. Connect ESP32-S3-Box3 to your computer using USB-C cable
2. The board should power on (you may see the screen light up)

### 6. Find Your Serial Port

**Linux:**
```bash
ls /dev/ttyUSB* /dev/ttyACM*
```
Usually `/dev/ttyUSB0` or `/dev/ttyACM0`

**macOS:**
```bash
ls /dev/cu.*
```
Usually `/dev/cu.usbserial-*`

**Windows:**
Check Device Manager → Ports (COM & LPT)  
Usually `COM3`, `COM4`, etc.

### 7. Flash the Firmware

Replace `/dev/ttyUSB0` with your actual port:

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

**Tip:** On first flash, you may need to hold the **BOOT** button on the board when connecting.

### 8. Enjoy!

After flashing completes, you should see:
- ✅ The display lights up with the UI
- ✅ Touch works to navigate tabs
- ✅ Files are listed in the Files tab
- ✅ You can tap files to open them

## What's Next?

### Try These Features:

1. **View an Image:**
   - Go to Files tab
   - Tap on `esp_logo.jpg` or `Death Star.jpg`
   - Image displays full screen
   - Tap anywhere to return

2. **Play Audio:**
   - Tap on `imperial_march.wav`
   - Listen to the audio (may take a moment to start)
   
3. **Read Text:**
   - Tap on `Readme.txt`
   - Read the text on screen

4. **Record Audio:**
   - Go to Recording tab
   - Tap "Start Recording"
   - Speak into the microphone
   - Tap "Stop Recording"
   - Tap "Play Recording" to hear it back

5. **Adjust Settings:**
   - Go to Settings tab
   - Slide the Volume control
   - Slide the Brightness control

### Add Your Own Files:

1. Place your files in `spiffs_content/`:
   - `.jpg` images (recommended max 800x600)
   - `.wav` audio files (16-bit PCM, mono/stereo)
   - `.txt` text files (UTF-8)

2. Rebuild and flash:
   ```bash
   idf.py build flash
   ```

## Troubleshooting Quick Fixes

### Build Failed?
```bash
# Clean and rebuild
idf.py fullclean
idf.py build
```

### Can't Connect to Board?
1. Try a different USB cable
2. Hold BOOT button while connecting
3. Check if driver is installed (Windows)

### Screen Not Working?
1. Unplug and replug USB cable
2. Re-flash firmware
3. Try: `idf.py erase-flash flash`

### No Audio?
1. Check volume in Settings tab
2. Set volume to 100%
3. Ensure file is valid WAV format

## Need More Help?

📖 See full [README.md](README.md) for detailed documentation  
🐛 [Report issues](https://github.com/DanielStuttgart/esp-audio-video/issues) on GitHub  
💬 Ask questions in [ESP32 Forum](https://www.esp32.com/)

---

**Happy Making! 🚀**
