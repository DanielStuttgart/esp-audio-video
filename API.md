# API Documentation

This document describes the API and internal structure of the ESP32-S3-Box3 Display Audio Photo application.

## Table of Contents

- [Application Structure](#application-structure)
- [Main Module](#main-module)
- [Display and Filesystem Module](#display-and-filesystem-module)
- [LVGL UI Components](#lvgl-ui-components)
- [Audio System](#audio-system)
- [File Types and Handling](#file-types-and-handling)
- [Configuration](#configuration)

## Application Structure

```
┌─────────────────────────────────────────────┐
│              app_main()                     │
│  - Entry point                              │
│  - Initialize all subsystems                │
└────────────────┬────────────────────────────┘
                 │
    ┌────────────┼────────────┐
    │            │            │
    ▼            ▼            ▼
┌─────────┐  ┌──────────┐  ┌──────────┐
│ SPIFFS  │  │ Display  │  │  Audio   │
│  Init   │  │   Init   │  │   Init   │
└─────────┘  └──────────┘  └──────────┘
                 │
    ┌────────────┼────────────┐
    │            │            │
    ▼            ▼            ▼
┌──────────┐ ┌─────────┐ ┌──────────┐
│  Files   │ │Recording│ │ Settings │
│   Tab    │ │   Tab   │ │   Tab    │
└──────────┘ └─────────┘ └──────────┘
```

## Main Module

**File:** `main/main.c`

### app_main()

Application entry point that initializes all subsystems.

```c
void app_main(void)
```

**Initialization Sequence:**
1. Mount SPIFFS filesystem
2. Initialize I2C bus
3. Initialize display and LVGL
4. Set display brightness
5. Create UI elements
6. Initialize file browser
7. Initialize audio codec

**Dependencies:**
- ESP-IDF FreeRTOS
- ESP-BSP (Board Support Package)
- LVGL graphics library

## Display and Filesystem Module

**Files:** `main/app_disp_fs.c`, `main/app_disp_fs.h`

### Public API

#### app_disp_lvgl_show()

Creates and displays the main tabbed UI.

```c
void app_disp_lvgl_show(void);
```

**Creates:**
- Tab view with 3 tabs
- Files browser tab
- Recording tab
- Settings tab

**UI Elements:**
- `lv_tabview`: Main tabbed interface
- Input groups for keyboard/encoder navigation
- Event handlers for tab switching

---

#### app_disp_fs_init()

Initializes the filesystem browser and populates file list.

```c
void app_disp_fs_init(void);
```

**Actions:**
- Scans SPIFFS mount point
- Lists all files in the root directory
- Creates clickable file list items
- Sets up file click handlers

**File System Path:** `/spiffs/` (defined as `BSP_SPIFFS_MOUNT_POINT`)

---

#### app_audio_init()

Initializes audio codec and I2S interface.

```c
void app_audio_init(void);
```

**Initializes:**
- ES8311 audio codec
- I2S peripheral
- Speaker output
- Microphone input (if available)

**Default Settings:**
- Sample rate: 22050 Hz
- Bits per sample: 16
- Volume: 70%

### Configuration Constants

```c
/* Default screen brightness (0-100%) */
#define APP_DISP_DEFAULT_BRIGHTNESS  (50)

/* Audio buffer size in bytes */
#define BUFFER_SIZE     (1024)

/* Audio sample rate in Hz */
#define SAMPLE_RATE     (22050)

/* Default volume (0-100%) */
#define DEFAULT_VOLUME  (70)

/* Recording length in buffers */
#define RECORDING_LENGTH (160)

/* Recorded file path */
#define REC_FILENAME    "/spiffs/recording.wav"
```

### Internal Functions

#### File Type Detection

```c
static app_file_type_t get_file_type(const char *filename);
```

Detects file type based on extension:
- `.jpg`, `.jpeg` → `APP_FILE_TYPE_IMG`
- `.wav` → `APP_FILE_TYPE_WAV`
- `.txt` → `APP_FILE_TYPE_TXT`
- Other → `APP_FILE_TYPE_UNKNOWN`

#### File Click Handler

```c
static void file_clicked_event_cb(lv_event_t *e);
```

Handles file selection events:
- Opens appropriate viewer based on file type
- Plays audio files
- Displays images
- Shows text content

#### Image Display

```c
static void show_image(const char *path);
```

**Process:**
1. Read JPEG file from SPIFFS
2. Decode using hardware JPEG decoder
3. Create LVGL image from decoded data
4. Display full-screen
5. Add touch handler to close

#### Audio Playback

```c
static void play_wav_file(const char *path);
```

**Process:**
1. Open WAV file
2. Parse WAV header
3. Extract audio parameters
4. Stream audio data to I2S
5. Handle playback completion

#### Text Display

```c
static void show_text(const char *path);
```

**Process:**
1. Read text file from SPIFFS
2. Create LVGL text area
3. Display content
4. Enable scrolling if needed

## LVGL UI Components

### Tab Structure

```
┌─────────────────────────────────────┐
│  Files  │ Recording │  Settings    │  ← Tab buttons
├─────────────────────────────────────┤
│                                     │
│         Tab Content Area            │
│                                     │
│                                     │
└─────────────────────────────────────┘
```

### Files Tab Components

- **File List** (`lv_list`)
  - Scrollable list of files
  - Each item shows filename
  - Click to open file
  
- **File Viewers** (dynamically created)
  - Image viewer for JPEGs
  - Audio player UI
  - Text reader

### Recording Tab Components

- **Record Button** (`lv_button`)
  - Starts/stops recording
  - Changes label based on state
  
- **Playback Button** (`lv_button`)
  - Plays recorded audio
  
- **Status Label** (`lv_label`)
  - Shows recording/playback status

### Settings Tab Components

- **Volume Slider** (`lv_slider`)
  - Range: 0-100
  - Default: 70
  - Real-time volume adjustment
  
- **Brightness Slider** (`lv_slider`)
  - Range: 0-100
  - Default: 50
  - Real-time brightness adjustment
  
- **Info Label** (`lv_label`)
  - Displays current settings

## Audio System

### Audio Codec Configuration

**Codec:** ES8311  
**Interface:** I2S  
**Mode:** Master/Slave (configurable)

### I2S Configuration

```c
i2s_config_t i2s_config = {
    .mode = I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_RX,
    .sample_rate = 22050,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    // ... other settings
};
```

### Audio Playback Flow

```
┌─────────────┐
│  WAV File   │
│  (SPIFFS)   │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Read Buffer │
│  (1024 B)   │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ I2S Driver  │
│    (DMA)    │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  ES8311     │
│   Codec     │
└──────┬──────┘
       │
       ▼
   Speaker 🔊
```

### Audio Recording Flow

```
 Microphone 🎤
       │
       ▼
┌─────────────┐
│  ES8311     │
│   Codec     │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ I2S Driver  │
│    (DMA)    │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│ Write Buffer│
│  (1024 B)   │
└──────┬──────┘
       │
       ▼
┌─────────────┐
│  WAV File   │
│  (SPIFFS)   │
└─────────────┘
```

## File Types and Handling

### Supported File Types

#### JPEG Images (.jpg, .jpeg)

**Requirements:**
- Baseline JPEG format
- Max recommended size: 800x600 pixels
- RGB color space

**Decoding:**
- Hardware JPEG decoder (ESP32-S3)
- Decoded to RGB565 format
- Displayed using LVGL image widget

**Memory:**
- File buffer: Dynamic allocation
- Decoded buffer: ~320x240x2 bytes max

#### WAV Audio (.wav)

**Supported Formats:**
- PCM uncompressed
- 16-bit samples
- Mono or Stereo
- Sample rates: 8000, 11025, 16000, 22050, 44100 Hz

**WAV Header Structure:**
```c
typedef struct __attribute__((packed)) {
    uint8_t ignore_0[22];
    uint16_t num_channels;      // 1=Mono, 2=Stereo
    uint32_t sample_rate;       // Hz
    uint8_t ignore_1[6];
    uint16_t bits_per_sample;   // 16
    uint8_t ignore_2[4];
    uint32_t data_size;         // Bytes
    uint8_t data[];             // Audio data
} dumb_wav_header_t;
```

#### Text Files (.txt)

**Requirements:**
- UTF-8 encoding
- Max file size limited by SPIFFS
- Line breaks: \n or \r\n

**Display:**
- LVGL text area widget
- Auto-scrolling if content > screen
- Monospace font

## Configuration

### Compile-Time Configuration

**sdkconfig.bsp.esp-box-3:**
- Flash size: 16MB
- PSRAM: Enabled (Octal, 80MHz)
- CPU frequency: 240MHz
- SPIFFS page size: 1024 bytes
- LVGL: Version 9, optimized settings

### Runtime Configuration

**Brightness:**
```c
bsp_display_brightness_set(0-100);
```

**Volume:**
```c
esp_codec_dev_set_out_vol(codec_dev, volume);
```

**LVGL Settings:**
- Can be modified via `lv_conf.h`
- Or via `menuconfig → LVGL configuration`

## Memory Management

### SPIFFS

**Total Size:** ~3MB (3,014,656 bytes)  
**Location:** 0x110000 - 0x400000

**Usage:**
- Sample images: ~40KB
- Sample audio: ~1.7MB
- Recorded audio: ~160KB per recording
- Text files: <1KB each

### Heap

**DRAM Heap:** Used for:
- LVGL objects
- File buffers
- Temporary allocations

**PSRAM:** Used for:
- Display framebuffer
- Large LVGL buffers
- Image decode buffers

### Stack

**Main Task:** 4KB (FreeRTOS default)  
**LVGL Task:** 4KB (configurable in BSP)

## Error Handling

### File Operations

All file operations check return values:
```c
FILE *f = fopen(path, "rb");
if (f == NULL) {
    ESP_LOGE(TAG, "Failed to open file: %s", path);
    return;
}
```

### Audio Operations

Audio errors are logged but don't crash app:
```c
if (codec_dev == NULL) {
    ESP_LOGE(TAG, "Audio codec not initialized");
    return;
}
```

### Display Operations

LVGL errors are handled internally, logged to console.

## Thread Safety

### LVGL Thread Safety

All LVGL calls must be protected:
```c
// LVGL operations run in LVGL task
// UI events are handled in event callbacks
// No manual locking needed (BSP handles it)
```

### File System Thread Safety

SPIFFS is thread-safe for read operations.  
Write operations should be serialized.

## Performance Considerations

### Display Performance

- **Target FPS:** 30-60
- **LVGL flush time:** ~16ms per frame
- **Optimization:** Use PSRAM for buffers

### Audio Performance

- **Latency:** ~50ms
- **Buffer time:** ~46ms per buffer (1024 bytes @ 22050 Hz)
- **DMA:** 3 buffers for smooth playback

### File System Performance

- **Read speed:** ~500 KB/s
- **Write speed:** ~300 KB/s
- **Optimization:** Use aligned buffers

## Extending the Application

### Adding New File Types

1. Add file type to enum:
   ```c
   typedef enum {
       APP_FILE_TYPE_UNKNOWN,
       APP_FILE_TYPE_TXT,
       APP_FILE_TYPE_IMG,
       APP_FILE_TYPE_WAV,
       APP_FILE_TYPE_MY_NEW_TYPE,  // Add here
   } app_file_type_t;
   ```

2. Update `get_file_type()`:
   ```c
   if (strstr(filename, ".myext")) {
       return APP_FILE_TYPE_MY_NEW_TYPE;
   }
   ```

3. Add handler in `file_clicked_event_cb()`:
   ```c
   case APP_FILE_TYPE_MY_NEW_TYPE:
       handle_my_new_type(path);
       break;
   ```

### Adding New UI Tabs

1. Increase tab count in `app_disp_lvgl_show()`
2. Create tab content function
3. Add to tab view
4. Create input group if needed

### Customizing Audio Settings

Modify in `app_disp_fs.c`:
```c
#define SAMPLE_RATE     (44100)  // Higher quality
#define DEFAULT_VOLUME  (80)     // Louder default
```

Rebuild and flash.

---

For more details, see the source code comments and [README.md](README.md).
