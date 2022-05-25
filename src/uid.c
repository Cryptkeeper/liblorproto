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
#include "../include/uid.h"

int lor_write_unit(lor_unit_t unit, uint8_t *b) {
  b[0] = unit;
  return 1;
}

int lor_read_unit(lor_unit_t *unit, const uint8_t *b) {
  *unit = b[0];
  return 1;
}

#define LOR_CHANNEL_GROUP_MULTIPLIER 64

#define LOR_CHANNEL_MASK_NO_OFFSET_MIN 0x80
#define LOR_CHANNEL_MASK_NO_OFFSET_MAX 0xBF

#define LOR_CHANNEL_MASK_OFFSET_MIN 0xC0
#define LOR_CHANNEL_MASK_OFFSET_MAX 0xFF

#define LOR_CHANNEL_MASK_OFFSET 0x80

int lor_write_channel(lor_channel_t channel, uint8_t *b) {
  const uint8_t offset = channel / LOR_CHANNEL_GROUP_MULTIPLIER;

  int n = 0;

  if (offset == 0) {
    b[n++] = channel + LOR_CHANNEL_MASK_NO_OFFSET_MIN;
  } else {
    b[n++] = (channel % 16) + LOR_CHANNEL_MASK_OFFSET_MIN;
    b[n++] = offset | LOR_CHANNEL_MASK_OFFSET;
  }

  return n;
}

int lor_read_channel(lor_channel_t *channel, const uint8_t *b) {
  const uint8_t idx = b[0];

  if (idx >= LOR_CHANNEL_MASK_NO_OFFSET_MIN && idx <= LOR_CHANNEL_MASK_NO_OFFSET_MAX) {
    *channel = idx - LOR_CHANNEL_MASK_NO_OFFSET_MIN;

    return 1;
  } else if (idx >= LOR_CHANNEL_MASK_OFFSET_MIN && idx <= LOR_CHANNEL_MASK_OFFSET_MAX) {
    const uint8_t offset = b[1] & ~LOR_CHANNEL_MASK_OFFSET;
    *channel = (idx - LOR_CHANNEL_MASK_OFFSET_MIN) + (offset * LOR_CHANNEL_GROUP_MULTIPLIER);

    return 2;
  } else {
    return 0;
  }
}

#define LOR_CHANNELSET_MASK_8L 0x40
#define LOR_CHANNELSET_MASK_8H 0x80

int lor_write_channelset(lor_channelset_t channelset, uint8_t *b) {
  int n = 0;

  const uint8_t bankL = (channelset.channels & 0x00FF);
  const uint8_t bankH = (channelset.channels & 0xFF00) >> 8;

  if (channelset.offset > 0) {
    if (bankL > 0 && bankH > 0) {
      b[n++] = channelset.offset;
    } else if (bankL > 0) {
      b[n++] = channelset.offset | LOR_CHANNELSET_MASK_8L;
    } else if (bankH > 0) {
      b[n++] = channelset.offset | LOR_CHANNELSET_MASK_8H;
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

int lor_read_channelset(lor_channelset_t *channelset, uint8_t cmd, const uint8_t *b) {
  int n = 0;

  if ((cmd & LOR_EFFECT_MASK_MULTIPART) == LOR_EFFECT_MASK_MULTIPART) {
    const uint8_t offset = b[n++];

    if ((offset & LOR_CHANNELSET_MASK_8H) == LOR_CHANNELSET_MASK_8H) {
      channelset->offset = (offset & ~LOR_CHANNELSET_MASK_8H);
      channelset->channels = (uint16_t)(b[n++] << 8);
    } else if ((offset & LOR_CHANNELSET_MASK_8L) == LOR_CHANNELSET_MASK_8L) {
      channelset->offset = (offset & ~LOR_CHANNELSET_MASK_8L);
      channelset->channels = (uint16_t)(b[n++]);
    } else if (offset > 0) {
      channelset->offset = offset;
      const uint8_t lsb = b[n++];
      channelset->channels = (uint16_t)(b[n++] << 8) | lsb;
    }
  } else if ((cmd & LOR_EFFECT_MASK_0) == LOR_EFFECT_MASK_0) {
    return n;
  } else if ((cmd & LOR_EFFECT_MASK_8L) == LOR_EFFECT_MASK_8L) {
    channelset->channels = (uint16_t)(b[n++]);
  } else if ((cmd & LOR_EFFECT_MASK_8H) == LOR_EFFECT_MASK_8H) {
    channelset->channels = (uint16_t)(b[n++]) << 8;
  } else if ((cmd & LOR_EFFECT_MASK_16) == LOR_EFFECT_MASK_16) {
    const uint8_t lsb = b[n++];
    channelset->channels = (uint16_t)(b[n++] << 8) | lsb;
  }

  return n;
}
