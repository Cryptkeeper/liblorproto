/*
 * MIT License
 *
 * Copyright (c) 2023 Nick Krecklow
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
#ifndef LIGHTORAMA_COMPRESS_H
#define LIGHTORAMA_COMPRESS_H

#include <stdint.h>

#include "effect.h"

enum {
    LOR_COMPRESSOR_SIZE = 16,
};

/*
 * An `LorCompressor` should be zero initialized before use.
 *
 * An optional base channel is provided by `lorCompressorSetBaseChannel`. This is used
 *  to relativize the `channel` parameter of any following `lorCompressorSetEffect` calls.
 *
 *  e.g. with a base channel of 16, the relative channel index of 0 would be 16, 1 would be 17 etc.
 *  A default base channel of 0 means all relative channel indexes match their absolute channel indexes.
 *
 * Each desired channel should be provided to the compressor via `lorCompressorSetEffect`.
 *  It supports up to 16 channels at a single time (named as `LOR_COMPRESSOR_SIZE`).
 *  This value is in reference to a 16-bit limitation within the LOR hardware protocol
 *  and not a library-level restriction.
 *
 * Finally, a call to `lorCompressorGenerate` will generate a set of `LorCompressorResult` values (up to 16)
 *  that can be used to send the compressed effects to the LOR hardware and return the count (if any).
 *
 * The caller can then encode these using `lorAppendChannelSetEffect` and related functions.
 *
 * See test/compress.c for usage example
 */
typedef struct LorCompressor {
    uint32_t baseChannel;
    /* channel that all effect indexes are relative to */
    LorEffect effects[LOR_COMPRESSOR_SIZE]; /* effect state per channel */
    union LorEffectArgs effectArgs[LOR_COMPRESSOR_SIZE]; /* optional args */
    uint16_t occupancySet; /* bitset of relative channels that have an effect */
} LorCompressor;

enum LorCompressorError {
    LOR_COMPRESSOR_OK           = 0,
    LOR_COMPRESSOR_OUT_OF_RANGE = -1,
    /* relative channel index is < base or > base + 16 */
    LOR_COMPRESSOR_UNALIGNED_BASE = 2,
    /* base channel index isn't multiple of 16 */
};

enum LorCompressorError lorCompressorSetBaseChannel(LorCompressor *compressor,
                                                    uint32_t baseChannel);

enum LorCompressorError lorCompressorSetEffect(LorCompressor *compressor,
                                               uint32_t channel,
                                               LorEffect effect,
                                               union LorEffectArgs args);

struct LorCompressorResult {
    LorChannelSet channelSet;
    LorEffect effect;
    union LorEffectArgs args;
};

int lorCompressorGenerate(
        const LorCompressor *compressor,
        struct LorCompressorResult results[LOR_COMPRESSOR_SIZE]);

#endif//LIGHTORAMA_COMPRESS_H
