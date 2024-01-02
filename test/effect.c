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
#undef NDEBUG
#include "lorproto/effect.h"

#include <assert.h>
#include <stdint.h>

static void test_effect_pair(const LorEffect effect,
                             const LorChannelFormat format,
                             const uint8_t expected) {
    uint8_t vstack[8] = {0};

    LorBuffer b = lorBufferInit(vstack, 8);

    lorAppendEffect(&b, effect, format);

    assert(b.offset == 1);
    assert(b.buffer[0] == expected);
}

int main(__attribute__((unused)) int argc,
         __attribute__((unused)) char **argv) {
    // tests encoding all effects values in each channel format and verifies
    // that the correct value is encoded using a known good table
    for (LorEffect effect = LOR_EFFECT_SET_LIGHTS;
         effect <= LOR_EFFECT_SET_DMX_INTENSITY; effect++) {
        test_effect_pair(effect, LOR_FORMAT_SINGLE, effect);
        test_effect_pair(effect, LOR_FORMAT_16, effect | 0x10);
        test_effect_pair(effect, LOR_FORMAT_8L, effect | 0x20);
        test_effect_pair(effect, LOR_FORMAT_8H, effect | 0x30);
        test_effect_pair(effect, LOR_FORMAT_UNIT, effect | 0x40);
        test_effect_pair(effect, LOR_FORMAT_MULTIPART, effect | 0x50);
    }

    return 0;
}
