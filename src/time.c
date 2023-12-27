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
#include "lightorama/time.h"

static uint16_t lorGetFadeTime(int deciseconds,
                               const LorIntensity start,
                               const LorIntensity end) {
    // clamp using `LOR_TIME_MIN_DS` and `LOR_TIME_MAX_DS`
    deciseconds = deciseconds < 1 ? 1 : deciseconds > 25 ? 25 : deciseconds;

    const LorIntensity dist = end >= start ? end - start : start - end;

    // this is the product of manually testing a large variable matrix
    // magic values may be slightly inaccurate
    const int sum = (dist * 256.0) / (deciseconds * 12.0);

    return sum & 0xFFFF;
}

void lorAppendFadeTime(LorBuffer *const b,
                       const int deciseconds,
                       const LorIntensity start,
                       const LorIntensity end) {
    const uint16_t time = lorGetFadeTime(deciseconds, start, end);

    const uint8_t b0 = time >> 8;
    const uint8_t b1 = time & 0xFF;

    if (b1 == 0) {
        lorAppendU8(b, b0 | 0x40);
        lorAppendU8(b, 1);
    } else if (b0 == 0) {
        lorAppendU8(b, b0 | 0x80);
        lorAppendU8(b, b1);
    } else {
        lorAppendU8(b, b0);
        lorAppendU8(b, b1);
    }
}
