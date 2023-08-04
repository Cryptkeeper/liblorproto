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

void lorEncodeUnit(const LorUnit unit, const LorWriteFn write) {
    write(unit);
}

void lorEncodeChannel(LorChannel channel, LorWriteFn write) {
    lorEncodeChannel2(channel, LOR_ALIGN_8, write);
}

// Light-O-Rama protocol requires all values be non-zero, so the highest order bit
// seems to be static true, which is applied via `LOR_CHANNEL_OPT`
#define LOR_CHANNEL_OPT            0b10000000                     /* 0x80 */
#define LOR_CHANNEL_OPT_HAS_OFFSET (0b01000000 | LOR_CHANNEL_OPT) /* 0xC0 */
#define LOR_CHANNEL_OPT_DATA_MASK  0b00111111                     /* 0x3F */

void lorEncodeChannel2(const LorChannel channel,
                       const LorChannelAlign align,
                       const LorWriteFn write) {
    // 2 highest bits are reserved as flags
    const int MaxChannel = 1 << 6;

    if (channel > MaxChannel) {
        const int offset = channel / MaxChannel;

        lorAssert(offset >= 0 && offset <= MaxChannel);

        write((channel % MaxChannel) | LOR_CHANNEL_OPT_HAS_OFFSET);
        write(offset | LOR_CHANNEL_OPT);
    } else {
        write(channel | LOR_CHANNEL_OPT);

        // some protocol usages of channel restructures require 16-bit alignment
        // this optionally pads using a magic protocol value
        if (align == LOR_ALIGN_16) write(0x81);
    }
}

static inline void lorGetChannelSetBanks(const LorChannelSet channelSet,
                                         uint8_t *const bankL,
                                         uint8_t *const bankH) {
    *bankL = channelSet.channelBits & 0xFF;
    *bankH = channelSet.channelBits >> 8;
}

LorChannelFormat lorGetChannelSetFormat(const LorChannelSet channelSet) {
    if (channelSet.offset > 0) return LOR_FORMAT_MULTIPART;

    uint8_t bankL, bankH;
    lorGetChannelSetBanks(channelSet, &bankL, &bankH);

    if (bankL > 0 && bankH > 0) return LOR_FORMAT_16;
    else if (bankL > 0)
        return LOR_FORMAT_8L;
    else if (bankH > 0)
        return LOR_FORMAT_8H;

    return LOR_FORMAT_SINGLE;
}

#define LOR_CHANNELSET_OPT_8L        0b01000000 /* 0x40 */
#define LOR_CHANNELSET_OPT_8H        0b10000000 /* 0x80 */
#define LOR_CHANNELSET_OPT_DATA_MASK 0b00111111 /* 0x3F */

void lorEncodeChannelSet(const LorChannelSet channelSet,
                         const LorWriteFn write) {
    // 6-bit unsigned int, max value of 64
    lorAssert(channelSet.offset <= 64);

    uint8_t bankL, bankH;
    lorGetChannelSetBanks(channelSet, &bankL, &bankH);

    if (channelSet.offset > 0) {
        if (bankL > 0 && bankH > 0) {
            write(channelSet.offset);
        } else if (bankL > 0) {
            write(channelSet.offset | LOR_CHANNELSET_OPT_8L);
        } else if (bankH > 0) {
            write(channelSet.offset | LOR_CHANNELSET_OPT_8H);
        }
    }

    if (bankL > 0) write(bankL);
    if (bankH > 0) write(bankH);
}
