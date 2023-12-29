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
#include "lightorama/model.h"

#undef NDEBUG
#include "lightorama/time.h"

#include "lightorama/intensity.h"

#include <assert.h>
#include <stdint.h>

int main(__attribute__((unused)) int argc,
         __attribute__((unused)) char **argv) {
    uint8_t buf[32] = {0};

    // all encoded fade time values should be two bytes in length and contain no
    //  null/zero values
    for (int ds = LOR_TIME_MIN_DS; ds <= LOR_TIME_MAX_DS; ds++) {
        LorBuffer b = lorBufferInit(buf, sizeof(buf));

        lorAppendFadeTime(&b, ds, LOR_INTENSITY_MIN, LOR_INTENSITY_MAX);

        assert(b.offset == 2);

        for (uint16_t i = 0; i < b.offset; i++)
            assert(buf[i] != 0);
    }

    return 0;
}
