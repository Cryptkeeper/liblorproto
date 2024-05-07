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
#include "lorproto/uid.h"

#include <assert.h>
#include <stdbool.h>

void lorAppendUnit(LorBuffer *const b, const LorUnit unit) {
    lorAppendU8(b, unit);
}

void lorAppendChannel(LorBuffer *const b, const LorChannel channel) {
    lorAppendAlignedChannel(b, channel, LOR_ALIGN_8);
}

// Light-O-Rama protocol requires all values be non-zero, so the highest order bit
// seems to be static true, which is applied via `LOR_CHANNEL_OPT`
#define LOR_CHANNEL_OPT            0x80 /* 0b10000000 */
#define LOR_CHANNEL_OPT_HAS_OFFSET 0xC0 /* 0b11000000 */
#define LOR_CHANNEL_OPT_DATA_MASK  0x3F /* 0b00111111 */

void lorAppendAlignedChannel(LorBuffer *const b,
                             const LorChannel channel,
                             const LorChannelAlign align) {
    // 2 highest bits are reserved as flags
    const int MaxChannel = 1 << 6;

    if (channel > MaxChannel) {
        const int offset = channel / MaxChannel;

        assert(offset >= 0 && offset < MaxChannel);

        lorAppendU8(b, channel % MaxChannel | LOR_CHANNEL_OPT_HAS_OFFSET);
        lorAppendU8(b, offset | LOR_CHANNEL_OPT);
    } else {
        lorAppendU8(b, channel | LOR_CHANNEL_OPT);

        // some protocol usages of channel restructures require 16-bit alignment
        // this optionally pads using a magic protocol value
        if (align == LOR_ALIGN_16) lorAppendU8(b, 0x81);
    }
}

LorChannelFormat lorGetChannelSetFormat(const LorChannelSet channelSet) {
    if (channelSet.offset > 0) return LOR_FORMAT_MULTIPART;

    const int count = __builtin_popcount(channelSet.channelBits);

    // FIXME: this is more likely an error scenario (an empty ChannelSets has no format)
    // legacy impl. returns _SINGLE by accident and we'll maintain that here for compat.
    // encoding an empty ChannelSets as _SINGLE will result in a 0-byte payload which
    // breaks up the message and results in a no-op, which means while it is wrong,
    // it is harmless and can be ignored.
    if (count == 0) return LOR_FORMAT_SINGLE;

    // detect truly single ChannelSets to avoid re-encoding using a bitset
    if (count == 1) return LOR_FORMAT_SINGLE;

    const bool bankL = (channelSet.channelBits & 0x00FF) > 0;
    const bool bankH = (channelSet.channelBits & 0xFF00) > 0;

    if (bankL && bankH) return LOR_FORMAT_16;
    if (bankL) return LOR_FORMAT_8L;
    if (bankH) return LOR_FORMAT_8H;

    assert(bankL || bankH);

    // popcount check above ensures at least one 8-bit bank is present
    // this should never be reached
    __builtin_unreachable();
}

#define LOR_CHANNELSET_OPT_8L        0x40 /* 0b01000000 */
#define LOR_CHANNELSET_OPT_8H        0x80 /* 0b10000000 */
#define LOR_CHANNELSET_OPT_DATA_MASK 0x3F /* 0b00111111 */

void lorAppendChannelSet(LorBuffer *const b, const LorChannelSet channelSet) {
    // 6-bit unsigned int, max value of 64
    assert(channelSet.offset < 64);

    const uint8_t bankL = channelSet.channelBits & 0xFF;
    const uint8_t bankH = channelSet.channelBits >> 8;

    if (channelSet.offset > 0) {
        if (bankL > 0 && bankH > 0) {
            lorAppendU8(b, channelSet.offset);
        } else if (bankL > 0) {
            lorAppendU8(b, channelSet.offset | LOR_CHANNELSET_OPT_8L);
        } else if (bankH > 0) {
            lorAppendU8(b, channelSet.offset | LOR_CHANNELSET_OPT_8H);
        }
    }

    if (bankL > 0) lorAppendU8(b, bankL);
    if (bankH > 0) lorAppendU8(b, bankH);
}
