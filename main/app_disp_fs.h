/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once


/* Default screen brightness (0-100%) */
#define APP_DISP_DEFAULT_BRIGHTNESS  (50)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Add and show LVGL objects on display
 * 
 * This function creates the main tabbed interface with:
 * - Files tab: Browse and open files
 * - Recording tab: Record and playback audio
 * - Settings tab: Adjust volume and brightness
 */
void app_disp_lvgl_show(void);

/**
 * @brief Initialize SPI Flash File System and show list of files on display
 * 
 * This function:
 * - Reads files from SPIFFS
 * - Populates the file list in the UI
 * - Sets up file selection handlers
 */
void app_disp_fs_init(void);

/**
 * @brief Initialize audio codec and playback system
 * 
 * This function initializes:
 * - ES8311 audio codec
 * - I2S audio interface
 * - Speaker and microphone
 */
void app_audio_init(void);

#ifdef __cplusplus
}
#endif
