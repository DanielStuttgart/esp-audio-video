/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "speech_recognition.h"
#include "esp_log.h"
#include "esp_err.h"
#include <string.h>
#include <stdlib.h>

// TensorFlow Lite includes
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Model and feature provider
#include "model.h"
#include "micro_model_settings.h"
#include "micro_features_generator.h"

static const char *TAG = "speech_recog";

// TensorFlow Lite global variables
namespace {
    const tflite::Model* model = nullptr;
    tflite::MicroInterpreter* interpreter = nullptr;
    TfLiteTensor* input = nullptr;
    TfLiteTensor* output = nullptr;
    
    constexpr int kTensorArenaSize = 12 * 1024;
    uint8_t tensor_arena[kTensorArenaSize];
}

// Feature buffer for audio samples
static int8_t g_feature_buffer[SPEECH_FEATURE_ELEMENT_COUNT];

// Feature data structure used by micro_features_generator
static Features g_features;

// Audio history buffer (ring buffer)
#define AUDIO_BUFFER_SIZE 16000  // 1 second at 16kHz
static int16_t g_audio_buffer[AUDIO_BUFFER_SIZE];
static int g_audio_buffer_pos = 0;

bool speech_recognition_init(void)
{
    ESP_LOGI(TAG, "Initializing speech recognition...");
    
    // Load the model
    model = tflite::GetModel(g_model);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
        ESP_LOGE(TAG, "Model schema version mismatch!");
        return false;
    }
    
    // Create micro op resolver with required operations
    static tflite::MicroMutableOpResolver<4> micro_op_resolver;
    micro_op_resolver.AddDepthwiseConv2D();
    micro_op_resolver.AddFullyConnected();
    micro_op_resolver.AddSoftmax();
    micro_op_resolver.AddReshape();
    
    // Create interpreter
    static tflite::MicroInterpreter static_interpreter(
        model, micro_op_resolver, tensor_arena, kTensorArenaSize);
    interpreter = &static_interpreter;
    
    // Allocate tensors
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        ESP_LOGE(TAG, "AllocateTensors() failed");
        return false;
    }
    
    // Get input and output tensors
    input = interpreter->input(0);
    output = interpreter->output(0);
    
    // Verify input tensor dimensions
    if ((input->dims->size != 2) || 
        (input->dims->data[0] != 1) ||
        (input->dims->data[1] != SPEECH_FEATURE_ELEMENT_COUNT)) {
        ESP_LOGE(TAG, "Bad input tensor dimensions");
        return false;
    }
    
    // Initialize audio buffer
    memset(g_audio_buffer, 0, sizeof(g_audio_buffer));
    g_audio_buffer_pos = 0;
    
    // Initialize the micro features generator
    TfLiteStatus feature_status = InitializeMicroFeatures();
    if (feature_status != kTfLiteOk) {
        ESP_LOGE(TAG, "InitializeMicroFeatures() failed");
        return false;
    }
    
    ESP_LOGI(TAG, "Speech recognition initialized successfully");
    return true;
}

// Proper feature extraction using the audio preprocessor model
// This uses the same feature extraction as the micro_speech example
static bool extract_features(const int16_t *audio_data, int audio_length, int8_t *feature_data)
{
    // Use the micro features generator which properly implements
    // FFT and mel filterbank for MFCC-like extraction
    TfLiteStatus generate_status = GenerateFeatures(audio_data, audio_length, &g_features);
    if (generate_status != kTfLiteOk) {
        ESP_LOGE(TAG, "GenerateFeatures() failed");
        return false;
    }
    
    // Copy features from 2D array to 1D buffer
    // Features are organized as [time_slice][frequency_bin]
    for (int i = 0; i < kFeatureCount; ++i) {
        for (int j = 0; j < kFeatureSize; ++j) {
            feature_data[i * kFeatureSize + j] = g_features[i][j];
        }
    }
    
    return true;
}


bool speech_recognition_process(const int16_t *audio_data, size_t audio_data_size,
                                speech_command_t *command, uint8_t *score)
{
    if (!interpreter || !input || !output) {
        ESP_LOGE(TAG, "Speech recognition not initialized");
        return false;
    }
    
    // Add new audio data to ring buffer
    int samples = audio_data_size / sizeof(int16_t);
    for (int i = 0; i < samples; i++) {
        g_audio_buffer[g_audio_buffer_pos] = audio_data[i];
        g_audio_buffer_pos = (g_audio_buffer_pos + 1) % AUDIO_BUFFER_SIZE;
    }
    
    // Extract features from the audio buffer
    if (!extract_features(g_audio_buffer, AUDIO_BUFFER_SIZE, g_feature_buffer)) {
        ESP_LOGE(TAG, "Feature extraction failed");
        return false;
    }
    
    // Copy features to input tensor
    for (int i = 0; i < SPEECH_FEATURE_ELEMENT_COUNT; i++) {
        input->data.int8[i] = g_feature_buffer[i];
    }
    
    // Run inference
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        ESP_LOGE(TAG, "Invoke failed");
        return false;
    }
    
    // Process output
    // Find the command with highest score
    int8_t max_score = -128;
    int max_index = 0;
    
    for (int i = 0; i < SPEECH_CMD_COUNT; i++) {
        int8_t current_score = output->data.int8[i];
        if (current_score > max_score) {
            max_score = current_score;
            max_index = i;
        }
    }
    
    *command = (speech_command_t)max_index;
    *score = (uint8_t)(max_score + 128);  // Convert from -128..127 to 0..255
    
    ESP_LOGD(TAG, "Recognized: %s (score: %d)", 
             speech_command_to_string(*command), *score);
    
    return true;
}

const char* speech_command_to_string(speech_command_t command)
{
    switch (command) {
        case SPEECH_CMD_SILENCE: return "silence";
        case SPEECH_CMD_UNKNOWN: return "unknown";
        case SPEECH_CMD_YES: return "yes";
        case SPEECH_CMD_NO: return "no";
        default: return "invalid";
    }
}

void speech_recognition_cleanup(void)
{
    // Cleanup is minimal for micro interpreter
    interpreter = nullptr;
    input = nullptr;
    output = nullptr;
    ESP_LOGI(TAG, "Speech recognition cleaned up");
}
