/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "model.h"

// This is a minimal placeholder model.
// In a production implementation, replace this with the actual trained
// TFLite micro speech model from esp-tflite-micro example.
// 
// To get the real model:
// 1. Clone https://github.com/espressif/esp-tflite-micro
// 2. Navigate to examples/micro_speech/main/
// 3. Copy the model.cc file which contains the trained model array
//
// The actual model is approximately 18KB in size.

alignas(8) const unsigned char g_model[] = {
    // Minimal TFLite model header (placeholder)
    // This won't work for actual inference but allows compilation
    0x1c, 0x00, 0x00, 0x00, 0x54, 0x46, 0x4c, 0x33,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00,
    // ... (minimal placeholder bytes)
};

const int g_model_len = sizeof(g_model);
