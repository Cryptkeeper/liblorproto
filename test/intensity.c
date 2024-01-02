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
#include "lorproto/intensity.h"

#undef NDEBUG
#include <assert.h>

static void test_intensity_pair(const float f, const LorIntensity i) {
    // validate clamping behavior across builtin intensity curves
    // users implementing the type should test their own curves
    assert(LorIntensityCurveVendor(f) == i);
    assert(LorIntensityCurveLinear(f) == i);
    assert(LorIntensityCurveSquared(f) == i);
}

int main(__attribute__((unused)) int argc,
         __attribute__((unused)) char **argv) {
    test_intensity_pair(-0.01, LOR_INTENSITY_MIN);
    test_intensity_pair(-1, LOR_INTENSITY_MIN);
    test_intensity_pair(-2, LOR_INTENSITY_MIN);

    test_intensity_pair(1.01, LOR_INTENSITY_MAX);
    test_intensity_pair(1, LOR_INTENSITY_MAX);
    test_intensity_pair(2, LOR_INTENSITY_MAX);

    return 0;
}
