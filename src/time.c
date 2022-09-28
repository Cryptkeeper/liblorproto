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
#include "../include/time.h"

#define LOR_CLAMPF(x, min, max) (x < min ? min : (x > max ? max : x))

lor_time_t lor_seconds_to_time(float seconds) {
  seconds = LOR_CLAMPF(seconds, LOR_TIME_MIN_SECONDS, LOR_TIME_MAX_SECONDS);
  return (lor_time_t)(LOR_TIME_MIN / (seconds / LOR_TIME_MIN_SECONDS));
}

float lor_time_to_seconds(lor_uint16_t time) {
  const float seconds = (LOR_TIME_MIN / (float)time) * LOR_TIME_MIN_SECONDS;
  return LOR_CLAMPF(seconds, LOR_TIME_MIN_SECONDS, LOR_TIME_MAX_SECONDS);
}

#define LOR_DURATION_MASK 0x8000

int lor_write_time(lor_time_t time, lor_uint8_t *b) {
  // include required 0x8000 protocol mask if the upper duration byte is zero
  const lor_uint16_t t = time | LOR_DURATION_MASK;
  b[0] = (t & 0xFF00) >> 8;
  b[1] = (t & 0x00FF);
  return 2;
}

int lor_read_time(lor_time_t *time, const lor_uint8_t *b) {
  // strip protocol mask bit (indicates the byte value is zero)
  lor_uint16_t t = ((lor_uint16_t)(b[0] << 8)) | b[1];
  t &= ~LOR_DURATION_MASK;
  *time = (lor_time_t)(t);
  return 2;
}
