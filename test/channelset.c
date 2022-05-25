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

int main() {
  uint8_t b[32];

  struct lor_channelset_t channelset, read_channelset;

  int offset;
  for (offset = 1; offset <= LOR_CHANNELSET_OFFSET_MAX; offset++) {
    int channels;
    for (channels = 0; channels < 16; channels++) {
      channelset.offset = offset;

      // rather than test each 2^16-1 value of `.channels` per offset, test each bit flag position,
      // instead creating 16 test cases per offset
      channelset.channels = 1 << channels;

      lor_write_channelset(channelset, b);

      uint8_t effect_mask;

      // copy of `lor_get_channelset_effect_mask` from src/effect.c
      // used to generate an effect mask valid within an encoded channelset context
      if (channelset.offset > 0) {
        effect_mask = LOR_EFFECT_MASK_MULTIPART;
      } else {
        const uint8_t bank0 = (channelset.channels & 0x00FF);
        const uint8_t bank1 = (channelset.channels & 0xFF00) >> 8;

        if (bank0 > 0x00 && bank1 > 0x00) {
          effect_mask = LOR_EFFECT_MASK_16;
        } else if (bank0 > 0) {
          effect_mask = LOR_EFFECT_MASK_8L;
        } else {
          effect_mask = LOR_EFFECT_MASK_8H;
        }
      }

      lor_read_channelset(&read_channelset, effect_mask, b);

      assert(channelset.offset == read_channelset.offset);
      assert(channelset.channels == read_channelset.channels);
    }
  }

  return 0;
}
