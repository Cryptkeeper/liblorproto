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
#include <stddef.h>

static void test_effect_encoding(lor_effect_t effect, void *effectStruct) {
  lor_uint8_t b[32];

  union lor_effect_any_t effect_read;
  int writtenb = lor_write_effect_struct(effect, effectStruct, b);

  assert(lor_read_effect_struct(effect, &effect_read, b) == writtenb);

  assert(lor_is_effect_eq(effect, effectStruct, &effect_read) == 1);

  // if `effectStruct` is non-NULL (has metadata arg), corrupt a portion of memory
  //  this tests for false positives in equality testing
  if (effectStruct != NULL) {
    // TODO: this assumes the first byte is member fields, could this fail with alignment padding?
    lor_uint8_t *ptr = (lor_uint8_t *)effectStruct;
    ptr[0] = ~ptr[0];

    assert(lor_is_effect_eq(effect, effectStruct, &effect_read) == 0);
  }
}

static void test_effect_setintensity(void) {
  static union lor_effect_any_t effectAny;
  effectAny.setIntensity.intensity = LOR_INTENSITY_MAX;

  test_effect_encoding(LOR_EFFECT_SET_INTENSITY, &effectAny);
}

static void test_effect_fade(void) {
  static union lor_effect_any_t effectAny;
  effectAny.fade.startIntensity = LOR_INTENSITY_MIN;
  effectAny.fade.endIntensity = LOR_INTENSITY_MAX;
  effectAny.fade.duration = lor_seconds_to_time(10);

  test_effect_encoding(LOR_EFFECT_FADE, &effectAny);
}

static void test_effect_pulse(void) {
  static union lor_effect_any_t effectAny;
  effectAny.pulse.halfInterval = lor_seconds_to_time(1);

  test_effect_encoding(LOR_EFFECT_PULSE, &effectAny);
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

  test_effect_encoding(LOR_EFFECT_SET_LIGHTS, NULL);
  test_effect_encoding(LOR_EFFECT_SET_OFF, NULL);
  test_effect_encoding(LOR_EFFECT_TWINKLE, NULL);
  test_effect_encoding(LOR_EFFECT_SHIMMER, NULL);

  test_effect_setintensity();
  test_effect_fade();
  test_effect_pulse();

  // test error return code for unmapped effects
  assert(lor_is_effect_eq((lor_effect_t)-1, NULL, NULL) == -1);

  return 0;
}
