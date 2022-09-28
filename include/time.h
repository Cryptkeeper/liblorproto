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
#ifndef LIGHTORAMA_TIME_H
#define LIGHTORAMA_TIME_H

#include "coretypes.h"

#define LOR_TIME_MAX         ((lor_time_t)0x8014)
#define LOR_TIME_MAX_SECONDS 25.0F

#define LOR_TIME_MIN         ((lor_time_t)0x13EB)
#define LOR_TIME_MIN_SECONDS 0.1F

/**
 * Encodes a float representation of seconds (`seconds`) into a `lor_time_t` value. `seconds` is
 * automatically clamped within the `LOR_TIME_MIN_SECONDS` and `LOR_TIME_MAX_SECONDS` bounds.
 *
 * `lor_time_t` is encoded as a uint16, and does not support the full precision of float. It is
 * recommended to use decisecond (0.1) increments only.
 */
lor_time_t lor_seconds_to_time(float seconds);

/**
 * Encodes a `lor_time_t` value (`time`) into a float representation of seconds. The resulting value
 * is automatically clamped within the `LOR_TIME_MIN_SECONDS` and `LOR_TIME_MAX_SECONDS` bounds.
 *
 * `lor_time_t` is encoded as a uint16, and does not support the full precision of float. It is
 * recommended to use decisecond (0.1) increments only.
 */
float lor_time_to_seconds(lor_time_t time);

int lor_write_time(lor_time_t time, lor_uint8_t *b);

int lor_read_time(lor_time_t *time, const lor_uint8_t *b);

#endif // LIGHTORAMA_TIME_H
