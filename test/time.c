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
#include "../include/lightorama.h"

#include <assert.h>
#include <stdio.h>

static void test_time_table(void) {
  assert(lor_seconds_to_time(0.1F) == 5099);
  assert(lor_seconds_to_time(0.5F) == 1019);
  assert(lor_seconds_to_time(1.0F) == 509);
  assert(lor_seconds_to_time(2.0F) == 254);
  assert(lor_seconds_to_time(25.0F) == 20);
}

static void test_time_interval_diff(float f) {
  const float real = lor_time_to_seconds(lor_seconds_to_time(f));

  // safely handle an offset between the expected seconds values
  // this is caused by the encoding scheme being lossy with precision
  const float diff = real > f ? real - f : f - real;

  printf("time: %f real: %f (∆%f)\n", f, real, diff);

  assert(diff < 0.01);
}

static void test_time_encoding(float s) {
  uint8_t b[32];

  lor_time_t time, read_time;
  int writtenb;

  time = lor_seconds_to_time(s);
  writtenb = lor_write_time(time, b);

  assert(lor_read_time(&read_time, b) == writtenb);

  assert(time == read_time);
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
  float test_intervals[5] = {0.1F, 0.5F, 1.0F, 2.0F, 25.0F};

  size_t i;
  for (i = 0; i < sizeof(test_intervals) / sizeof(float); i++) {
    const float f = test_intervals[i];

    test_time_interval_diff(f);
    test_time_encoding(f);
  }

  test_time_table();

  return 0;
}
