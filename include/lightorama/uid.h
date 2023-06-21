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
#ifndef LIGHTORAMA_UID_H
#define LIGHTORAMA_UID_H

#include "coretypes.h"

#define LOR_UNIT_MIN ((lor_unit_t)0x01)
#define LOR_UNIT_MAX ((lor_unit_t)0xF0)

// https://github.com/Cryptkeeper/lightorama-protocol/blob/master/PROTOCOL.md#unit-ids
#define LOR_UNIT_ALL ((lor_unit_t)0xFF)

int lor_write_unit(lor_unit_t unit, lor_uint8_t *b);

int lor_read_unit(lor_unit_t *unit, const lor_uint8_t *b);

int lor_write_channel(lor_channel_t channel, lor_uint8_t *b);

int lor_read_channel(lor_channel_t *channel, const lor_uint8_t *b);

#define LOR_CHANNELSET_OFFSET_MAX ((1 << 6) - 1)

struct lor_channelset_t {
  lor_uint8_t offset;
  lor_uint16_t channels;
}; // typedef'd by coretypes.h

int lor_write_channelset(lor_channelset_t channelset, lor_uint8_t *b);

int lor_read_channelset(lor_channelset_t *channelset, lor_uint8_t cmd, const lor_uint8_t *b);

#define LOR_OFFSET_OPT_16        0x10
#define LOR_OFFSET_OPT_8H        0x20
#define LOR_OFFSET_OPT_8L        0x30
#define LOR_OFFSET_OPT_UNIT      0x40
#define LOR_OFFSET_OPT_MULTIPART 0x50

#endif // LIGHTORAMA_UID_H
