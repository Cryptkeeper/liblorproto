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
#include "lightorama/compress.h"

#include <stdbool.h>

enum LorCompressorError lorCompressorSetBaseChannel(
        LorCompressor *const compressor,
        const uint32_t baseChannel) {
    if (baseChannel % LOR_COMPRESSOR_SIZE) return LOR_COMPRESSOR_UNALIGNED_BASE;

    compressor->baseChannel = baseChannel;

    return LOR_COMPRESSOR_OK;
}

#define OCCUPANCY_BIT(index) (1 << index)

enum LorCompressorError lorCompressorSetEffect(LorCompressor *const compressor,
                                               const uint32_t channel,
                                               LorEffect effect,
                                               union LorEffectArgs args) {
    if (compressor->baseChannel > channel) return LOR_COMPRESSOR_OUT_OF_RANGE;

    // relativize index according to the base index for the compressor range
    const uint32_t idx = channel - compressor->baseChannel;

    // ensure index is within the compressor's single shot range
    if (idx >= LOR_COMPRESSOR_SIZE) return LOR_COMPRESSOR_OUT_OF_RANGE;

    compressor->effects[idx]    = effect;
    compressor->effectArgs[idx] = args;

    compressor->occupancySet |= OCCUPANCY_BIT(idx);

    return LOR_COMPRESSOR_OK;
}

static bool lorCompressorEffectMatches(const LorCompressor *const compressor,
                                       const int a,
                                       const int b) {
    if (compressor->effects[a] != compressor->effects[b]) return false;

    const union LorEffectArgs aArgs = compressor->effectArgs[a];
    const union LorEffectArgs bArgs = compressor->effectArgs[b];

    switch (compressor->effects[a]) {
        case LOR_EFFECT_SET_INTENSITY:
            return aArgs.setIntensity.intensity ==
                   bArgs.setIntensity.intensity;

        case LOR_EFFECT_FADE:
            return aArgs.fade.startIntensity == bArgs.fade.startIntensity &&
                   aArgs.fade.endIntensity == bArgs.fade.endIntensity &&
                   aArgs.fade.duration == bArgs.fade.duration;

        case LOR_EFFECT_PULSE:
            return aArgs.pulse.halfInterval == bArgs.pulse.halfInterval;

        default:
            return false;
    }
}

// returns a bitset of all configured channels in the compressor that have a matching effect state
uint16_t lorCompressorGetMatches(const LorCompressor *const compressor,
                                 const int compare) {
    uint16_t matches = 0;

    for (int i = 0; i < LOR_COMPRESSOR_SIZE; i++) {
        const int bit = OCCUPANCY_BIT(i);
        if (!(compressor->occupancySet & bit)) continue;/* unused channel */

        if (lorCompressorEffectMatches(compressor, i, compare)) matches |= bit;
    }

    return matches;
}

int lorCompressorGenerate(const LorCompressor *const compressor,
                          struct LorCompressorResult results[
                              LOR_COMPRESSOR_SIZE]) {
    if (!compressor->occupancySet) return 0;

    uint16_t updatedBits = 0;/* bitset of channels that have been processed */
    int generated        = 0;/* number of results generated */

    for (int i = 0; i < LOR_COMPRESSOR_SIZE; i++) {
        // skip unused channels
        if (!(compressor->occupancySet & OCCUPANCY_BIT(i))) continue;

        // find matching channels and exclude any that were previously handled
        const uint16_t matches =
                lorCompressorGetMatches(compressor, i) & ~updatedBits;

        if (!matches) continue;/* no new matches */

        updatedBits |= matches;/* mark channels as handled */

        // store the resulting bitmask set and unique effect
        results[generated++] = (struct LorCompressorResult){
                .channelSet = (LorChannelSet){
                        .offset = compressor->baseChannel / LOR_COMPRESSOR_SIZE,
                        .channelBits = matches,
                },
                .effect = compressor->effects[i],
                .args = compressor->effectArgs[i],
        };

        // break early if all channels are updated
        if (updatedBits == compressor->occupancySet) break;
    }

    return generated;
}
