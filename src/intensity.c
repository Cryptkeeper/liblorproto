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
#include "../include/intensity.h"

int lor_write_intensity(lor_intensity_t intensity, uint8_t *buf) {
  buf[0] = intensity;
  return 1;
}

int lor_read_intensity(lor_intensity_t *intensity, const uint8_t *buf) {
  *intensity = buf[0];
  return 1;
}

#define LOR_CLAMPF(val, min, max) ((val) < (min) ? (min) : ((val) > (max)) ? (max) : (val))
#define LOR_INTENSITY_RANGE       ((lor_intensity_t)(LOR_INTENSITY_MAX - LOR_INTENSITY_MIN))

lor_intensity_t lor_intensity_curve_vendor(float normal) {
  const float clamped = LOR_CLAMPF(normal, 0, 1);
  if (clamped == 0) {
    return LOR_INTENSITY_MIN;
  } else if (clamped == 1) {
    return LOR_INTENSITY_MAX;
  } else {
    return ((lor_intensity_t)0xE4) - (lor_intensity_t)(clamped * 2);
  }
}

lor_intensity_t lor_intensity_curve_linear(float normal) {
  return ((lor_intensity_t)(LOR_CLAMPF(normal, 0, 1) * LOR_INTENSITY_RANGE)) + LOR_INTENSITY_MIN;
}

lor_intensity_t lor_intensity_curve_squared(float normal) {
  const float clamped = LOR_CLAMPF(normal, 0, 1);
  const float squared = 1 - (1 - clamped) * (1 - clamped);
  return ((lor_intensity_t)(squared * LOR_INTENSITY_RANGE)) + LOR_INTENSITY_MIN;
}
