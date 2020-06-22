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
#ifndef LIGHTORAMA_IO_H
#define LIGHTORAMA_IO_H

#include "protocol.h"

#include <stddef.h>

size_t lor_write_heartbeat(unsigned char *ptr);

size_t lor_write_brightness(lor_brightness_t brightness, unsigned char *ptr);

size_t lor_write_brightnessf(float normal, lor_brightness_curve_t brightness_curve, unsigned char *ptr);

size_t lor_write_duration(lor_duration_t duration, unsigned char *ptr);

size_t lor_write_durationf(float seconds, unsigned char *ptr);

size_t lor_write_channel(LORChannelType type, lor_channel_t channel, unsigned char *ptr);

size_t lor_write_channel_action(lor_unit_t unit, LORChannelType type, lor_channel_t channel, lor_channel_action_t action, unsigned char *ptr);

size_t lor_write_channel_fade(lor_unit_t unit, LORChannelType type, lor_channel_t channel, lor_brightness_t from, lor_brightness_t to, lor_duration_t duration, unsigned char *ptr);

size_t lor_write_channel_fade_with(lor_unit_t unit, LORChannelType type, lor_channel_t channel, lor_channel_action_t foreground_action, lor_brightness_t from, lor_brightness_t to, lor_duration_t duration, unsigned char *ptr);

size_t lor_write_channel_set_brightness(lor_unit_t unit, LORChannelType type, lor_channel_t channel, lor_brightness_t brightness, unsigned char *ptr);

size_t lor_write_unit_action(lor_unit_t unit, lor_unit_action_t action, unsigned char *ptr);

#endif //LIGHTORAMA_IO_H
