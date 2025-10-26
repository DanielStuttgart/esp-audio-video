/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef MICRO_MODEL_SETTINGS_H_
#define MICRO_MODEL_SETTINGS_H_

// Model input/output settings
constexpr int kFeatureSliceSize = 40;
constexpr int kFeatureSliceCount = 49;
constexpr int kFeatureElementCount = (kFeatureSliceSize * kFeatureSliceCount);

constexpr int kCategoryCount = 4;
constexpr int kSilenceIndex = 0;
constexpr int kUnknownIndex = 1;

#endif  // MICRO_MODEL_SETTINGS_H_
