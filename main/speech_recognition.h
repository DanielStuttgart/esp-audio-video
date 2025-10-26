/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Audio configuration
#define SPEECH_SAMPLE_RATE      16000
#define SPEECH_AUDIO_LENGTH_MS  1000
#define SPEECH_FEATURE_SLICE_SIZE 40
#define SPEECH_FEATURE_SLICE_COUNT 49
#define SPEECH_FEATURE_ELEMENT_COUNT (SPEECH_FEATURE_SLICE_SIZE * SPEECH_FEATURE_SLICE_COUNT)

// Command labels
typedef enum {
    SPEECH_CMD_SILENCE = 0,
    SPEECH_CMD_UNKNOWN = 1,
    SPEECH_CMD_YES = 2,
    SPEECH_CMD_NO = 3,
    SPEECH_CMD_COUNT
} speech_command_t;

/**
 * @brief Initialize speech recognition system
 * 
 * @return true if initialization successful, false otherwise
 */
bool speech_recognition_init(void);

/**
 * @brief Process audio buffer and perform speech recognition
 * 
 * @param audio_data Pointer to audio data buffer (16-bit PCM)
 * @param audio_data_size Size of audio data in bytes
 * @param[out] command Recognized command
 * @param[out] score Confidence score (0-255)
 * @return true if command recognized, false otherwise
 */
bool speech_recognition_process(const int16_t *audio_data, size_t audio_data_size, 
                                speech_command_t *command, uint8_t *score);

/**
 * @brief Get string representation of command
 * 
 * @param command Speech command enum value
 * @return const char* String name of command
 */
const char* speech_command_to_string(speech_command_t command);

/**
 * @brief Cleanup speech recognition resources
 */
void speech_recognition_cleanup(void);

#ifdef __cplusplus
}
#endif
