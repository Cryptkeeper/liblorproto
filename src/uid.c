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

#include <stdbool.h>

LorResult
lorEncodeUnit(const LorUnit unit, unsigned char *const b, const size_t bSize) {
    if (b != NULL) {
        if (bSize < 1) return LorErrOutOfBuffer;

        b[0] = unit;
    }

    return 1;
}

// Light-O-Rama protocol requires all values be non-zero, so the highest order bit
// seems to be static true, which is applied via `LOR_CHANNEL_OPT`
#define LOR_CHANNEL_OPT            0b10000000                     /* 0x80 */
#define LOR_CHANNEL_OPT_HAS_OFFSET (0b01000000 | LOR_CHANNEL_OPT) /* 0xC0 */
#define LOR_CHANNEL_OPT_DATA_MASK  0b00111111                     /* 0x3F */

LorResult lorEncodeChannel(const LorChannel channel,
                           unsigned char *const b,
                           const size_t bSize) {
    // 2 highest bits are reserved as flags
    const int MaxChannel = 1 << 6;

    const bool requiresOffset = channel > MaxChannel;
    const int size = requiresOffset ? 2 : 1;

    if (b != NULL) {
        if (bSize < size) return LorErrOutOfBuffer;

        if (requiresOffset) {
            const int offset = channel / MaxChannel;

            if (offset < 0 || offset > MaxChannel) return LorErrInvalidChannel;

            b[0] = (channel % MaxChannel) | LOR_CHANNEL_OPT_HAS_OFFSET;
            b[1] = offset | LOR_CHANNEL_OPT;
        } else {
            b[0] = channel | LOR_CHANNEL_OPT;
        }
    }

    return size;
}

#define LOR_CHANNELSET_OPT_8L        0b01000000 /* 0x40 */
#define LOR_CHANNELSET_OPT_8H        0b10000000 /* 0x80 */
#define LOR_CHANNELSET_OPT_DATA_MASK 0b00111111 /* 0x3F */

LorResult lorEncodeChannelSet(const LorChannelSet channelSet,
                              unsigned char *const b,
                              const size_t bSize) {
    // 6-bit unsigned int, max value of 64
    if (channelSet.offset > 64) return LorErrInvalidArg;

    const uint8_t bankL = channelSet.channelBits & 0xFF;
    const uint8_t bankH = channelSet.channelBits >> 8;

    int size = 0;

    if (bankL > 0) size++;
    if (bankH > 0) size++;
    if (channelSet.offset > 0) size++;

    if (b != NULL) {
        if (bSize < size) return LorErrOutOfBuffer;

        if (channelSet.offset > 0) {
            if (bankL > 0 && bankH > 0) {
                b[0] = channelSet.offset;
            } else if (bankL > 0) {
                b[0] = channelSet.offset | LOR_CHANNELSET_OPT_8L;
            } else if (bankH > 0) {
                b[0] = channelSet.offset | LOR_CHANNELSET_OPT_8H;
            }
        }

        if (bankL > 0) b[size - 2] = bankL;
        if (bankH > 0) b[size - 1] = bankH;
    }

    return size;
}
