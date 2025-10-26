# Speech Recognition Setup

This document describes how to set up speech recognition in this project.

## Overview

The speech recognition feature uses TensorFlow Lite Micro to recognize voice commands:
- **silence** - No speech detected
- **unknown** - Speech detected but not a recognized command
- **yes** - "Yes" command
- **no** - "No" command

## Architecture

The implementation follows the architecture from [esp-tflite-micro](https://github.com/espressif/esp-tflite-micro):

1. **Audio Capture**: Records audio at 16kHz, 16-bit, mono using the onboard microphone
2. **Ring Buffer**: Maintains a sliding window of audio samples (1 second)
3. **Feature Extraction**: Converts audio to spectral features (simplified energy-based approach - see note below)
4. **TFLite Inference**: Runs the micro speech model on extracted features
5. **Post-processing**: Identifies the most confident command
6. **Display**: Shows recognized commands on the UI

**Note on Feature Extraction**: The current implementation uses a simplified energy-based feature extraction instead of full MFCC (Mel-Frequency Cepstral Coefficients). This may result in lower accuracy compared to the reference implementation. For production use, consider integrating the proper MFCC feature extraction from esp-tflite-micro's `feature_provider.cc`.

## Getting the TFLite Model

⚠️ **IMPORTANT**: The current `model.cc` file contains only a placeholder model that won't work for actual speech recognition.

To get the real trained model:

### Option 1: Use Pre-trained Model from esp-tflite-micro

1. Clone the esp-tflite-micro repository:
   ```bash
   git clone https://github.com/espressif/esp-tflite-micro
   cd esp-tflite-micro/examples/micro_speech
   ```

2. Copy the model file:
   ```bash
   cp main/model.cc /path/to/esp-audio-video/main/model.cc
   ```

3. The model is approximately 18KB and contains the trained TFLite model for micro speech.

### Option 2: Train Your Own Model

Follow the [TensorFlow Lite Micro Speech tutorial](https://github.com/tensorflow/tflite-micro/tree/main/tensorflow/lite/micro/examples/micro_speech) to train a custom model with your own dataset.

## Usage

1. Navigate to the "Speech" tab on the display
2. Tap "START RECOGNITION"
3. The device will listen for up to 30 seconds
4. Speak one of the commands: "yes" or "no"
5. The recognized command appears on the screen
6. Recognition automatically stops after 30 seconds or tap the button again to stop early

## Configuration

Key parameters in `speech_recognition.h`:
- `SPEECH_SAMPLE_RATE`: 16000 Hz (do not change, model is trained for this rate)
- `SPEECH_AUDIO_LENGTH_MS`: 1000 ms audio window
- `SPEECH_FEATURE_SLICE_SIZE`: 40 features per time slice
- `SPEECH_FEATURE_SLICE_COUNT`: 49 time slices

Memory requirements:
- Tensor arena: 10KB (configurable in `speech_recognition.cpp`)
- Audio buffer: 32KB (16000 samples * 2 bytes)
- Feature buffer: ~2KB

## Troubleshooting

### "Model schema version mismatch" error
The TFLite model version doesn't match the TFLite Micro library version. Ensure you're using compatible versions.

### Low recognition accuracy
1. Check microphone placement and ambient noise
2. Speak clearly and at normal volume
3. Ensure the model is the correct trained model (not the placeholder)
4. The confidence threshold can be adjusted in `app_disp_fs.c` in the `recog_speech()` function:
   - Look for `if (command != SPEECH_CMD_SILENCE && score > 150)`
   - The threshold `150` (out of 255, or ~59%) can be lowered for more sensitivity or raised for higher confidence
   - Lower values (e.g., 100-130) will detect more commands but may have more false positives
   - Higher values (e.g., 180-200) will only report very confident matches

### Out of memory errors
1. Increase the task stack size in `speech_event_cb` (currently 8192 bytes)
2. Reduce the `kTensorArenaSize` if needed (minimum ~10KB)

### No commands recognized
1. Verify the model file is correctly copied
2. Check logs for initialization errors
3. Ensure microphone is working (test with Record tab)

## Implementation Details

### Files Added

- `speech_recognition.h/cpp`: Main speech recognition logic
- `model.h/cc`: TFLite model data (needs to be replaced with actual model)
- `micro_model_settings.h`: Model input/output configuration

### Integration Points

- `app_disp_fs.c`: UI and task management
  - `recog_speech()`: Main recognition task with 30-second timeout
  - `speech_event_cb()`: Button click handler
  - `app_disp_lvgl_show_speech()`: UI layout

### Audio Processing Pipeline

```
Microphone → esp_codec_dev_read() → recording_buffer (1024 bytes)
           → Ring buffer (16000 samples) → Feature extraction
           → TFLite input tensor → Inference → Command output
```

## Future Improvements

- [ ] **Add proper MFCC feature extraction**: Current implementation uses a simplified energy-based approach instead of full MFCC (Mel-Frequency Cepstral Coefficients). The simplifications include:
  - No FFT computation (uses simple energy calculation instead)
  - No mel filterbank application
  - No DCT (Discrete Cosine Transform) for cepstral coefficients
  - Impact: Lower accuracy compared to proper MFCC, may struggle with similar-sounding words
  - For production use, integrate proper MFCC from esp-tflite-micro's feature_provider
- [ ] Support for more commands (requires retraining model)
- [ ] Adjustable sensitivity/confidence threshold in UI
- [ ] Audio visualization during recording
- [ ] Command history/log

## References

- [esp-tflite-micro](https://github.com/espressif/esp-tflite-micro)
- [TensorFlow Lite Micro](https://www.tensorflow.org/lite/microcontrollers)
- [Micro Speech Example](https://github.com/tensorflow/tflite-micro/tree/main/tensorflow/lite/micro/examples/micro_speech)
