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

static void test_interval_diffs(void) {
  float intervals[5] = {0.1F, 0.5F, 1.0F, 2.0F, 25.0F};

  size_t i;
  for (i = 0; i < sizeof(intervals) / sizeof(float); i++) {
    const float input = intervals[i];
    const float output = lor_time_to_seconds(lor_seconds_to_time(input));

    // safely handle an offset between the expected seconds values
    // this is caused by the encoding scheme being lossy with precision
    const float diff = output > input ? output - input : input - output;

    printf("%f %f (%f)\n", input, output, diff);

    assert(diff < 0.01);
  }
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
  test_time_table();
  test_interval_diffs();

  return 0;
}
