/*
 * MIT License
 *
 * Copyright (c) 2020 Nick Krecklow
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
#include "brightness_curve.h"
#include "math.h"

lor_brightness_t lor_brightness_curve_linear(float normal) {
    return ((lor_brightness_t) (clampf(normal, 0, 1) * LOR_BRIGHTNESS_RANGE)) + LOR_BRIGHTNESS_MIN;
}

lor_brightness_t lor_brightness_curve_squared(float normal) {
    const float clamped = clampf(normal, 0, 1);
    const float squared = 1 - (1 - clamped) * (1 - clamped);
    return ((lor_brightness_t) (squared * LOR_BRIGHTNESS_RANGE)) + LOR_BRIGHTNESS_MIN;
}

lor_brightness_t lor_brightness_curve_xlights(float normal) {
    const float clamped = clampf(normal, 0, 1);
    if (clamped == 0) {
        return LOR_BRIGHTNESS_MIN;
    } else if (clamped == 1) {
        return LOR_BRIGHTNESS_MAX;
    } else {
        // Rounding behavior is inherited from xLights implementation and is considered unsafe
        // Clang-Tidy: Casting (double + 0.5) to integer leads to incorrect rounding
        return 0xE4 - (unsigned char) (clamped * 100 + 0.5F) * 2;
    }
}