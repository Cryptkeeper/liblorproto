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

static const union LorEffectArgs gMaxBrightness = {
        .setIntensity = {
                .intensity = LOR_INTENSITY_MAX,
        }
};

static const union LorEffectArgs gMinBrightness = {
        .setIntensity = {
                .intensity = LOR_INTENSITY_MIN,
        }
};

static void test_usage_errors(void) {
    LorCompressor c = {0};

    // set unaligned base channel
    assert(lorCompressorSetBaseChannel(&c, 1) == LOR_COMPRESSOR_UNALIGNED_BASE);

    // set aligned base channels
    assert(lorCompressorSetBaseChannel(&c, 0) == LOR_COMPRESSOR_OK);
    assert(lorCompressorSetBaseChannel(&c, 16) == LOR_COMPRESSOR_OK);

    assert(
            lorCompressorSetEffect(&c, 0, LOR_EFFECT_SET_INTENSITY,
                gMaxBrightness) ==
            LOR_COMPRESSOR_OUT_OF_RANGE);/* too low */

    assert(
            lorCompressorSetEffect(&c, 32, LOR_EFFECT_SET_INTENSITY,
                gMaxBrightness) ==
            LOR_COMPRESSOR_OUT_OF_RANGE);/* too high */

    assert(c.occupancySet == 0);// check bitset is empty due to expected errors

    // test overlapping set
    assert(
            lorCompressorSetEffect(&c, 17, LOR_EFFECT_SET_INTENSITY,
                gMaxBrightness) == LOR_COMPRESSOR_OK);

    assert(
            lorCompressorSetEffect(&c, 17, LOR_EFFECT_SET_INTENSITY,
                gMaxBrightness) == LOR_COMPRESSOR_OK);

    // test occupancy set reflects changes
    assert((c.occupancySet & 0x2) == 0x2);

    assert(
            lorCompressorSetEffect(&c, 24, LOR_EFFECT_SET_INTENSITY,
                gMinBrightness) ==
            LOR_COMPRESSOR_OK);

    assert(
            lorCompressorSetEffect(&c, 25, LOR_EFFECT_SET_INTENSITY,
                gMinBrightness) ==
            LOR_COMPRESSOR_OK);

    // test occupancy set reflects changes
    assert((c.occupancySet & 0x302) == 0x302);
}

static void test_multiple_results_usage(void) {
    LorCompressor c = {
            .baseChannel = 16,
    };

    const LorEffect effect = LOR_EFFECT_SET_INTENSITY;

    // configure two sequential channels with same effect in upper and lower 8 bit blocks
    // this enables easy verification of the results
    assert(
            lorCompressorSetEffect(&c, 16, effect, gMaxBrightness) ==
            LOR_COMPRESSOR_OK);
    assert(
            lorCompressorSetEffect(&c, 17, effect,
                gMaxBrightness) ==
            LOR_COMPRESSOR_OK);

    assert(
            lorCompressorSetEffect(&c, 24, effect,
                gMinBrightness) ==
            LOR_COMPRESSOR_OK);
    assert(
            lorCompressorSetEffect(&c, 25, effect,
                gMinBrightness) ==
            LOR_COMPRESSOR_OK);

    // test generated output
    struct LorCompressorResult results[LOR_COMPRESSOR_SIZE] = {0};
    const int generated = lorCompressorGenerate(&c, results);

    assert(generated == 2);/* 2 effects generated to match min/maxBrightness */

    assert(results[0].effect == effect);
    assert(
            results[0].args.setIntensity.intensity == gMaxBrightness.
            setIntensity.intensity);

    assert(results[0].channelSet.offset == 1);
    assert(results[0].channelSet.channelBits == 0x3);
    assert(
            lorGetChannelSetFormat(results[0].channelSet) ==
            LOR_FORMAT_MULTIPART);

    assert(results[1].effect == effect);
    assert(
            results[1].args.setIntensity.intensity == gMinBrightness.
            setIntensity.intensity);

    assert(results[1].channelSet.offset == 1);
    assert(results[1].channelSet.channelBits == 0x300);
    assert(
            lorGetChannelSetFormat(results[1].channelSet) ==
            LOR_FORMAT_MULTIPART);
}

static void test_empty_usage(void) {
    const LorCompressor c = {0};

    struct LorCompressorResult results[LOR_COMPRESSOR_SIZE] = {0};

    const int generated = lorCompressorGenerate(&c, results);

    assert(generated == 0);
}

static void test_full_compression(void) {
    LorCompressor c = {0};

    // test output with a single effect that compresses into all available outputs
    // this also uses zero as the first channel to test that offsets remain zero
    assert(lorCompressorSetBaseChannel(&c, 0) == LOR_COMPRESSOR_OK);

    for (int i = 0; i < LOR_COMPRESSOR_SIZE; i++) {
        assert(
                lorCompressorSetEffect(&c, i, LOR_EFFECT_SET_INTENSITY,
                    gMaxBrightness) == LOR_COMPRESSOR_OK);
    }

    struct LorCompressorResult results[LOR_COMPRESSOR_SIZE] = {0};

    const int generated = lorCompressorGenerate(&c, results);

    assert(generated == 1);

    assert(results[0].effect == LOR_EFFECT_SET_INTENSITY);
    assert(
            results[0].args.setIntensity.intensity == gMaxBrightness.
            setIntensity.intensity);

    // validate channel set includes all 16 channels starting at zero
    assert(results[0].channelSet.offset == 0);
    assert(results[0].channelSet.channelBits == 0xFFFF);

    assert(lorGetChannelSetFormat(results[0].channelSet) == LOR_FORMAT_16);
}

static void test_8l_compression(void) {
    struct LorCompressorResult results[LOR_COMPRESSOR_SIZE] = {0};

    LorCompressor c = {0};

    // validate only using the lower 8 bits
    c.occupancySet = 0;

    for (int i = 0; i < LOR_COMPRESSOR_SIZE / 2; i++) {
        assert(
                lorCompressorSetEffect(&c, i, LOR_EFFECT_SET_INTENSITY,
                    gMaxBrightness) == LOR_COMPRESSOR_OK);
    }

    const int generated = lorCompressorGenerate(&c, results);

    assert(generated == 1);

    assert(results[0].effect == LOR_EFFECT_SET_INTENSITY);
    assert(
            results[0].args.setIntensity.intensity == gMaxBrightness.
            setIntensity.intensity);

    assert(lorGetChannelSetFormat(results[0].channelSet) == LOR_FORMAT_8L);

    // validate channel set includes only 8 channels starting at zero
    assert(results[0].channelSet.offset == 0);
    assert(results[0].channelSet.channelBits == 0x00FF);
}

static void test_8h_compression(void) {
    struct LorCompressorResult results[LOR_COMPRESSOR_SIZE] = {0};

    LorCompressor c = {0};

    // validate only using the upper 8 bits
    c.occupancySet = 0;

    for (int i = 0; i < LOR_COMPRESSOR_SIZE / 2; i++) {
        assert(
                lorCompressorSetEffect(&c, LOR_COMPRESSOR_SIZE/2 + i,
                    LOR_EFFECT_SET_INTENSITY,
                    gMaxBrightness) == LOR_COMPRESSOR_OK);
    }

    const int generated = lorCompressorGenerate(&c, results);

    assert(generated == 1);

    assert(results[0].effect == LOR_EFFECT_SET_INTENSITY);
    assert(
            results[0].args.setIntensity.intensity == gMaxBrightness.
            setIntensity.intensity);

    // validate channel set includes only 8 channels starting at 8
    assert(results[0].channelSet.offset == 0);
    assert(results[0].channelSet.channelBits == 0xFF00);

    assert(lorGetChannelSetFormat(results[0].channelSet) == LOR_FORMAT_8H);
}

int main(__attribute__((unused)) int argc,
         __attribute__((unused)) char **argv) {
    test_usage_errors();
    test_multiple_results_usage();
    test_empty_usage();
    test_full_compression();
    test_8l_compression();
    test_8h_compression();

    return 0;
}
