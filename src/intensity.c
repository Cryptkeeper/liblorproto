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
#include "lightorama/intensity.h"

void lorEncodeIntensity(const LorIntensity intensity, const LorWriteFn write) {
    write(intensity);
}

#define LOR_CLAMPF(val, min, max)                                              \
    ((val) < (min) ? (min) : ((val) > (max)) ? (max) : (val))

#define LOR_INTENSITY_RANGE                                                    \
    ((LorIntensity) (LOR_INTENSITY_MAX - LOR_INTENSITY_MIN))

inline LorIntensity LorIntensityCurveVendor(float normal) {
    const float clamped = LOR_CLAMPF(normal, 0, 1);

    if (clamped == 0) {
        return LOR_INTENSITY_MIN;
    } else if (clamped == 1) {
        return LOR_INTENSITY_MAX;
    } else {
        return ((LorIntensity) 0xE4) - (LorIntensity) (clamped * 200);
    }
}

inline LorIntensity LorIntensityCurveLinear(float normal) {
    return ((LorIntensity) (LOR_CLAMPF(normal, 0, 1) * LOR_INTENSITY_RANGE)) +
           LOR_INTENSITY_MIN;
}

inline LorIntensity LorIntensityCurveSquared(float normal) {
    const float clamped = LOR_CLAMPF(normal, 0, 1);
    const float squared = 1 - (1 - clamped) * (1 - clamped);

    return ((LorIntensity) (squared * LOR_INTENSITY_RANGE)) + LOR_INTENSITY_MIN;
}
