/*
 * SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

/**
 * @file
 * @brief ESP32-S3-Box3 Display, Audio and Photo Example
 * 
 * This example demonstrates the multimedia capabilities of the ESP32-S3-Box3:
 * - Display: 320x240 IPS LCD with LVGL graphics
 * - Touch: Capacitive touch panel for user input
 * - Audio: Play WAV files and record audio
 * - Storage: SPIFFS filesystem for storing media files
 * 
 * Features:
 * - Browse files stored in SPIFFS
 * - View JPEG images with hardware decoding
 * - Play WAV audio files through speaker
 * - Record audio using built-in microphone
 * - Read text files on screen
 * - Adjust volume and brightness settings
 * 
 * Hardware:
 * - ESP32-S3-Box3 development board
 * - 320x240 pixel IPS display
 * - ES8311 audio codec
 * - Built-in speaker and microphone
 * 
 * @example https://github.com/DanielStuttgart/esp-audio-video
 */

#include "esp_log.h"
#include "bsp/esp-bsp.h"
#include "app_disp_fs.h"

static const char *TAG = "example";

/**
 * @brief Main application entry point
 * 
 * Initialization sequence:
 * 1. Mount SPIFFS filesystem
 * 2. Initialize I2C bus (for touch and audio)
 * 3. Initialize display and LVGL
 * 4. Set display brightness
 * 5. Create UI elements
 * 6. Initialize file system browser
 * 7. Initialize audio codec
 */
void app_main(void)
{
    ESP_LOGI(TAG, "ESP32-S3-Box3 Display Audio Photo Example");
    ESP_LOGI(TAG, "Starting initialization...");

    /* Initialize and mount SPIFFS filesystem */
    ESP_LOGI(TAG, "Mounting SPIFFS...");
    bsp_spiffs_mount();

    /* Initialize I2C bus (required for touch panel and audio codec) */
    ESP_LOGI(TAG, "Initializing I2C...");
    bsp_i2c_init();

    /* Initialize display and LVGL graphics library */
    ESP_LOGI(TAG, "Starting display...");
    bsp_display_start();

    /* Set default display brightness */
    ESP_LOGI(TAG, "Setting display brightness to %d%%", APP_DISP_DEFAULT_BRIGHTNESS);
    bsp_display_brightness_set(APP_DISP_DEFAULT_BRIGHTNESS);

    /* Create and show LVGL UI objects on display */
    ESP_LOGI(TAG, "Creating UI...");
    app_disp_lvgl_show();

    /* Initialize file system browser and populate file list */
    ESP_LOGI(TAG, "Initializing file system...");
    app_disp_fs_init();

    /* Initialize audio codec and I2S interface */
    ESP_LOGI(TAG, "Initializing audio...");
    app_audio_init();

    ESP_LOGI(TAG, "Example initialization complete!");
    ESP_LOGI(TAG, "Touch the screen to interact with the application.");
}
