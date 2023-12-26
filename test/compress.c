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
#include "lightorama/coretypes.h"

#undef NDEBUG
#include "lightorama/intensity.h"
#include "lightorama/compress.h"

#include <assert.h>

int main(__attribute__((unused)) int argc,
         __attribute__((unused)) char **argv) {
    LorCompressor c = {0};

    // set unaligned base channel
    assert(lorCompressorSetBaseChannel(&c, 1) == LOR_COMPRESSOR_UNALIGNED_BASE);

    // set aligned base channels
    assert(lorCompressorSetBaseChannel(&c, 0) == LOR_COMPRESSOR_OK);
    assert(lorCompressorSetBaseChannel(&c, 16) == LOR_COMPRESSOR_OK);

    const union LorEffectArgs maxBrightness = {
            .setIntensity = {
                    .intensity = LOR_INTENSITY_MAX,
            }
    };

    const union LorEffectArgs minBrightness = {
            .setIntensity = {
                    .intensity = LOR_INTENSITY_MIN,
            }
    };

    const LorEffect effect = LOR_EFFECT_SET_INTENSITY;

    assert(
            lorCompressorSetEffect(&c, 0, effect, maxBrightness) ==
            LOR_COMPRESSOR_OUT_OF_RANGE);/* too low */

    assert(
            lorCompressorSetEffect(&c, 32, effect,
                maxBrightness) ==
            LOR_COMPRESSOR_OUT_OF_RANGE);/* too high */

    assert(c.occupancySet == 0);/* check bitset is empty */

    // configure two sequential channels with same effect in upper and lower 8 bit blocks
    // this enables easy verification of the results
    assert(
            lorCompressorSetEffect(&c, 16, effect, maxBrightness) ==
            LOR_COMPRESSOR_OK);
    assert(
            lorCompressorSetEffect(&c, 17, effect,
                maxBrightness) ==
            LOR_COMPRESSOR_OK);

    // test overlapping set
    assert(
            lorCompressorSetEffect(&c, 17, effect,
                maxBrightness) ==
            LOR_COMPRESSOR_OK);

    assert((c.occupancySet & 0x3) == 0x3);

    assert(
            lorCompressorSetEffect(&c, 24, effect,
                minBrightness) ==
            LOR_COMPRESSOR_OK);

    assert(
            lorCompressorSetEffect(&c, 25, effect,
                minBrightness) ==
            LOR_COMPRESSOR_OK);

    assert((c.occupancySet & 0x303) == 0x303);

    // test generated output
    struct LorCompressorResult results[LOR_COMPRESSOR_SIZE] = {0};
    int generated = lorCompressorGenerate(&c, results);

    assert(generated == 2);/* 2 effects generated to match min/maxBrightness */

    assert(results[0].effect == effect);
    assert(results[0].channelSet.offset == 1);
    assert(
            results[0].args.setIntensity.intensity == maxBrightness.
            setIntensity.intensity);

    assert(results[1].effect == effect);
    assert(results[1].channelSet.offset == 1);
    assert(
            results[1].args.setIntensity.intensity == minBrightness.
            setIntensity.intensity);

    // test generated output with "empty" compressor
    c.occupancySet = 0;

    generated = lorCompressorGenerate(&c, results);

    assert(generated == 0);

    // test output with a single effect that compresses into all available outputs
    // this also uses zero as the first channel to test that offsets remain zero
    assert(lorCompressorSetBaseChannel(&c, 0) == LOR_COMPRESSOR_OK);

    for (int i = 0; i < 16; i++) {
        assert(
                lorCompressorSetEffect(&c, i, LOR_EFFECT_SET_INTENSITY,
                    maxBrightness) == LOR_COMPRESSOR_OK);
    }

    generated = lorCompressorGenerate(&c, results);

    assert(generated == 1);
    assert(results[0].effect == LOR_EFFECT_SET_INTENSITY);
    assert(
            results[0].args.setIntensity.intensity == maxBrightness.
            setIntensity.intensity);

    // validate channel set includes all 16 channels starting at zero
    assert(results[0].channelSet.offset == 0);
    assert(results[0].channelSet.channelBits == 0xFFFF);

    return 0;
}
