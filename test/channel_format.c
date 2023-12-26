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
#include "lightorama/uid.h"

#undef NDEBUG
#include <assert.h>

static void test_channelset_result(const unsigned offset,
                                   const unsigned bits,
                                   const LorChannelFormat format) {
    const LorChannelSet cs = {
            .offset = offset,
            .channelBits = bits,
    };

    assert(lorGetChannelSetFormat(cs) == format);
}

int main(__attribute__((unused)) int argc,
         __attribute__((unused)) char **argv) {
    test_channelset_result(0, 0x00FF, LOR_FORMAT_8L);
    test_channelset_result(0, 0x0001, LOR_FORMAT_8L);

    test_channelset_result(0, 0xFF00, LOR_FORMAT_8H);
    test_channelset_result(0, 0x1000, LOR_FORMAT_8H);

    test_channelset_result(0, 0xFFFF, LOR_FORMAT_16);
    test_channelset_result(0, 0x1001, LOR_FORMAT_16);

    // anything with an offset > 0 should be multipart
    test_channelset_result(1, 0x00FF, LOR_FORMAT_MULTIPART);
    test_channelset_result(1, 0xFF00, LOR_FORMAT_MULTIPART);
    test_channelset_result(1, 0xFFFF, LOR_FORMAT_MULTIPART);
    test_channelset_result(1, 0x0001, LOR_FORMAT_MULTIPART);
    test_channelset_result(1, 0x1000, LOR_FORMAT_MULTIPART);
    test_channelset_result(1, 0x1001, LOR_FORMAT_MULTIPART);

    return 0;
}
