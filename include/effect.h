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

#include "coretypes.h"

#define LOR_EFFECT_SET_LIGHTS    ((lor_effect_t)0x01)
#define LOR_EFFECT_SET_OFF       ((lor_effect_t)0x02)
#define LOR_EFFECT_SET_INTENSITY ((lor_effect_t)0x03)
#define LOR_EFFECT_FADE          ((lor_effect_t)0x04)
#define LOR_EFFECT_PULSE         ((lor_effect_t)0x05)
#define LOR_EFFECT_TWINKLE       ((lor_effect_t)0x06)
#define LOR_EFFECT_SHIMMER       ((lor_effect_t)0x07)

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

/**
 * Compares two effect metadata structures to test if their member fields (within `aes` & `bes`) are
 * equal. Assumes `aes` & `bes` are not NULL if the value of `effect` requires an associated
 * metadata struct. If a metadata struct is NOT required, the pointers will be ignored by the
 * function and may be non-NULL.
 *
 * Metadata structs may take the form of any `struct lor_effect_*_t` type, or `union
 * lor_effect_any_t`. The two parameters do not need to match types for the purposes of the
 * comparison.
 *
 * @param effect
 * @param aes A pointer to a `struct lor_effect_*_t` or `union lor_effect_any_t`, if the `effect`
 * value requires an associated metadata struct
 * @param bes A pointer to a `struct lor_effect_*_t` or `union lor_effect_any_t` used to compare for
 * equality
 * @return 1 if equal, 0 if not, -1 if `effect` is an unknown/unsupported value
 */
int lor_is_effect_eq(lor_effect_t effect, const void *aes, const void *bes);

#endif // LIGHTORAMA_EFFECT_H
