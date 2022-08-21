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

static void test_channelset_encoding(int offset, int channels, uint8_t *b) {
  struct lor_channelset_t channelset, read_channelset;
  uint8_t encode_opts;

  channelset.offset = offset;

  // rather than test each 2^16-1 value of `.channels` per offset, test each bit flag position,
  // instead creating 16 test cases per offset
  channelset.channels = 1 << channels;

  lor_write_channelset(channelset, b);

  // copy of `lor_get_channelset_opts` from src/effect.c
  // used to generate a set of bitflag options valid within an encoded channelset context
  if (channelset.offset > 0) {
    encode_opts = LOR_OFFSET_OPT_MULTIPART;
  } else {
    const uint8_t bank0 = (channelset.channels & 0x00FF);
    const uint8_t bank1 = (channelset.channels & 0xFF00) >> 8;

    if (bank0 > 0x00 && bank1 > 0x00) {
      encode_opts = LOR_OFFSET_OPT_16;
    } else if (bank0 > 0) {
      encode_opts = LOR_OFFSET_OPT_8L;
    } else {
      encode_opts = LOR_OFFSET_OPT_8H;
    }
  }

  lor_read_channelset(&read_channelset, encode_opts, b);

  assert(channelset.offset == read_channelset.offset);
  assert(channelset.channels == read_channelset.channels);
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
  uint8_t b[32];

  int offset;
  for (offset = 1; offset <= LOR_CHANNELSET_OFFSET_MAX; offset++) {
    int channels;
    for (channels = 0; channels < 16; channels++) {
      test_channelset_encoding(offset, channels, b);
    }
  }

  return 0;
}
