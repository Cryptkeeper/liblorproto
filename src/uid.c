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
#include "lightorama/uid.h"

int lor_write_unit(lor_unit_t unit, lor_uint8_t *b) {
  b[0] = unit;
  return 1;
}

int lor_read_unit(lor_unit_t *unit, const lor_uint8_t *b) {
  *unit = b[0];
  return 1;
}

// Light-O-Rama protocol requires all values be non-zero, so the highest order bit
// seems to be static true, which is applied via `LOR_CHANNEL_OPT`
#define LOR_CHANNEL_OPT            0b10000000                     /* 0x80 */
#define LOR_CHANNEL_OPT_HAS_OFFSET (0b01000000 | LOR_CHANNEL_OPT) /* 0xC0 */
#define LOR_CHANNEL_OPT_DATA       0b00111111                     /* 0x3F */
#define LOR_CHANNEL_OPT_DATA_MAX   (1 << 6)                       /* 64 */

int lor_write_channel(lor_channel_t channel, lor_uint8_t *b) {
  // TODO: implement overflow protection on offset math (/,%)?
  const lor_uint8_t offset = channel / LOR_CHANNEL_OPT_DATA_MAX;

  int n = 0;

  if (offset == 0) {
    b[n++] = channel | LOR_CHANNEL_OPT;
  } else {
    b[n++] = (channel % LOR_CHANNEL_OPT_DATA_MAX) | LOR_CHANNEL_OPT_HAS_OFFSET;
    b[n++] = offset | LOR_CHANNEL_OPT;
  }

  return n;
}

int lor_read_channel(lor_channel_t *channel, const lor_uint8_t *b) {
  const lor_uint8_t opts = b[0];

  if (opts & LOR_CHANNEL_OPT) {
    if (opts & LOR_CHANNEL_OPT_HAS_OFFSET) {
      const lor_uint8_t offset = b[1] & ~LOR_CHANNEL_OPT;
      *channel = (opts & LOR_CHANNEL_OPT_DATA) + (offset * LOR_CHANNEL_OPT_DATA_MAX);

      return 2;
    } else {
      *channel = opts & ~LOR_CHANNEL_OPT;

      return 1;
    }
  }

  return 0;
}

#define LOR_CHANNELSET_OPT_8L   0b01000000 /* 0x40 */
#define LOR_CHANNELSET_OPT_8H   0b10000000 /* 0x80 */
#define LOR_CHANNELSET_OPT_DATA 0b00111111 /* 0x3F */

int lor_write_channelset(lor_channelset_t channelset, lor_uint8_t *b) {
  int n = 0;

  const lor_uint8_t bankL = (channelset.channels & 0x00FF);
  const lor_uint8_t bankH = (channelset.channels & 0xFF00) >> 8;

  if (channelset.offset > 0) {
    const lor_uint8_t offset = channelset.offset & LOR_CHANNELSET_OPT_DATA;

    if (bankL > 0 && bankH > 0) {
      b[n++] = offset;
    } else if (bankL > 0) {
      b[n++] = offset | LOR_CHANNELSET_OPT_8L;
    } else if (bankH > 0) {
      b[n++] = offset | LOR_CHANNELSET_OPT_8H;
    }
  }

  if (bankL > 0) {
    b[n++] = bankL;
  }
  if (bankH > 0) {
    b[n++] = bankH;
  }

  return n;
}

int lor_read_channelset(lor_channelset_t *channelset, lor_uint8_t cmd, const lor_uint8_t *b) {
  int n = 0;

  if (cmd & LOR_OFFSET_OPT_MULTIPART) {
    const lor_uint8_t opts = b[n++];

    if (opts & LOR_CHANNELSET_OPT_8H) {
      channelset->offset = opts & LOR_CHANNELSET_OPT_DATA;
      channelset->channels = (lor_uint16_t)(b[n++] << 8);
    } else if (opts & LOR_CHANNELSET_OPT_8L) {
      channelset->offset = opts & LOR_CHANNELSET_OPT_DATA;
      channelset->channels = (lor_uint16_t)(b[n++]);
    } else if (opts > 0) {
      channelset->offset = opts & LOR_CHANNELSET_OPT_DATA;
      const lor_uint8_t lsb = b[n++];
      channelset->channels = (lor_uint16_t)(b[n++] << 8) | lsb;
    }
  } else if (cmd & LOR_OFFSET_OPT_UNIT) {
    return n;
  } else if (cmd & LOR_OFFSET_OPT_8L) {
    channelset->channels = (lor_uint16_t)(b[n++]);
  } else if (cmd & LOR_OFFSET_OPT_8H) {
    channelset->channels = (lor_uint16_t)(b[n++]) << 8;
  } else if (cmd & LOR_OFFSET_OPT_16) {
    const lor_uint8_t lsb = b[n++];
    channelset->channels = (lor_uint16_t)(b[n++] << 8) | lsb;
  }

  return n;
}
