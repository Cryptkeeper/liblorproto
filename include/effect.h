/*
 * MIT License
 *
 * Copyright (c) 2022 Nick Krecklow
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef LIGHTORAMA_EFFECT_H
#define LIGHTORAMA_EFFECT_H

#include <stdint.h>

#include "intensity.h"
#include "time.h"
#include "uid.h"

typedef enum lor_effect_t {
  LOR_EFFECT_SET_LIGHTS = 0x01,
  LOR_EFFECT_SET_OFF = 0x02,
  LOR_EFFECT_SET_INTENSITY = 0x03,
  LOR_EFFECT_FADE = 0x04,
  LOR_EFFECT_PULSE = 0x05,
  LOR_EFFECT_TWINKLE = 0x06,
  LOR_EFFECT_SHIMMER = 0x07,
} lor_effect_t;

struct lor_effect_setintensity_t {
  lor_intensity_t intensity;
};

struct lor_effect_fade_t {
  lor_intensity_t startIntensity;
  lor_intensity_t endIntensity;
  lor_time_t duration;
};

struct lor_effect_pulse_t {
  lor_time_t halfInterval;
};

union lor_effect_any_t {
  struct lor_effect_setintensity_t setIntensity;
  struct lor_effect_fade_t fade;
  struct lor_effect_pulse_t pulse;
};

int lor_write_effect_struct(lor_effect_t effect, const void *effectStruct, uint8_t *b);

int lor_read_effect_struct(lor_effect_t effect,
                           union lor_effect_any_t *effectStruct,
                           const uint8_t *b);

#endif // LIGHTORAMA_EFFECT_H
