/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MODEL_H_
#define MODEL_H_

#include <stdint.h>

// This is a placeholder model array. In a real implementation, this would be
// replaced with the actual trained TFLite model for micro speech recognition.
// The model should be generated from the esp-tflite-micro example.

// For now, we'll use a minimal placeholder model
// A real model would be much larger (typically 18KB for micro_speech)
extern const unsigned char g_model[];
extern const int g_model_len;

#endif  // MODEL_H_
