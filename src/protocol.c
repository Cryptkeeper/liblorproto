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
#include "../include/protocol.h"
#include "math.h"

lor_duration_t lor_duration_of(float seconds) {
    const float seconds_clamped = clampf(seconds, LOR_DURATION_SECONDS_MIN, LOR_DURATION_SECONDS_MAX);
    const lor_duration_t duration = (lor_duration_t) (((float) LOR_DURATION_MIN / (seconds_clamped / LOR_DURATION_SECONDS_MIN)) + 0.5F);
    if (duration > 0xFF) {
        return duration;
    } else {
        return 0x8000u | duration;
    }
}

lor_channel_action_magic_t lor_get_channel_action_magic(enum lor_channel_type_t type) {
    switch (type) {
        case LOR_CHANNEL_MASK8:
            return LOR_CHANNEL_ACTION_MAGIC_MASK8;
        case LOR_CHANNEL_MASK16:
            return LOR_CHANNEL_ACTION_MAGIC_MASK16;
        default:
            return 0x00;
    }
}
