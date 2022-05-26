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
#ifndef LIGHTORAMA_INTENSITY_H
#define LIGHTORAMA_INTENSITY_H

#include <stdint.h>

typedef uint8_t lor_intensity_t;

#define LOR_INTENSITY_MIN ((lor_intensity_t)0xF0)
#define LOR_INTENSITY_MAX ((lor_intensity_t)0x01)

int lor_write_intensity(lor_intensity_t intensity, uint8_t *buf);

int lor_read_intensity(lor_intensity_t *intensity, const uint8_t *buf);

typedef lor_intensity_t (*lor_intensity_curve_t)(float normal);

lor_intensity_t lor_intensity_curve_vendor(float normal);

lor_intensity_t lor_intensity_curve_linear(float normal);

lor_intensity_t lor_intensity_curve_squared(float normal);

#endif // LIGHTORAMA_INTENSITY_H
