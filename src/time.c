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
#include "lightorama/time.h"

#define LOR_CLAMPF(x, min, max) (x < min ? min : (x > max ? max : x))

LorTime lorSecondsToTime(const float seconds) {
    const float s =
            LOR_CLAMPF(seconds, LOR_TIME_MIN_SECONDS, LOR_TIME_MAX_SECONDS);

    return (LorTime) (LOR_TIME_MIN / (s / LOR_TIME_MIN_SECONDS));
}

float lorTimeToSeconds(const LorTime time) {
    const float seconds = (LOR_TIME_MIN / (float) time) * LOR_TIME_MIN_SECONDS;

    return LOR_CLAMPF(seconds, LOR_TIME_MIN_SECONDS, LOR_TIME_MAX_SECONDS);
}

#define LOR_DURATION_MASK 0x8000

void lorAppendTime(LorBuffer *const b, LorTime time) {
    const uint16_t bits = time | LOR_DURATION_MASK;

    lorAppendU8(b, bits >> 8);
    lorAppendU8(b, bits & 0xFF);
}
