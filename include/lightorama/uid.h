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

#define LOR_UNIT_MIN ((LorUnit) 0x01)
#define LOR_UNIT_MAX ((LorUnit) 0xF0)

// https://github.com/Cryptkeeper/lightorama-protocol/blob/master/PROTOCOL.md#unit-ids
#define LOR_UNIT_ALL ((LorUnit) 0xFF)

void lorEncodeUnit(LorUnit unit, LorWriteFn write);

bool lorEncodeChannel(LorChannel channel, LorWriteFn write);

bool lorEncodeChannel2(LorChannel channel, int align, LorWriteFn write);

typedef enum LorChannelFormat {
    LOR_FORMAT_SINGLE    = 0x00, /* 0b0000 */
    LOR_FORMAT_16        = 0x10, /* 0b0001 */
    LOR_FORMAT_8L        = 0x20, /* 0b0010 */
    LOR_FORMAT_8H        = 0x30, /* 0b0011 */
    LOR_FORMAT_UNIT      = 0x40, /* 0b0100 */
    LOR_FORMAT_MULTIPART = 0x50, /* 0b0101 */
} LorChannelFormat;

LorChannelFormat lorGetChannelSetFormat(LorChannelSet channelSet);

bool lorEncodeChannelSet(LorChannelSet channelSet, LorWriteFn write);

#endif// LIGHTORAMA_UID_H
