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
#ifndef LIGHTORAMA_PROTOCOL_H
#define LIGHTORAMA_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>

/**
 * Magic Numbers
 */
static const uint8_t LOR_MAGIC_FLUSH = 0x00;
static const uint8_t LOR_MAGIC_AND = 0x81;
static const uint8_t LOR_MAGIC_HEARTBEAT = 0x56;
static const uint8_t LOR_MAGIC_CHANNEL_ID_MASK = 0x80;

/**
 * Units
 */
typedef uint8_t lor_unit_t;

static const lor_unit_t LOR_UNIT_ID_MIN = 0x01;
static const lor_unit_t LOR_UNIT_ID_MAX = 0xF0;

static const lor_unit_t LOR_UNIT_ID_BROADCAST = 0xFF;
static const lor_unit_t LOR_UNIT_ID_COMPUTER = 0xFE;

/**
 * Channels
 */
typedef enum lor_channel_type_t {
    LOR_CHANNEL_ID = 1,
    LOR_CHANNEL_MASK8 = 8,
    LOR_CHANNEL_MASK16 = 16,
} LORChannelType;

typedef uint16_t lor_channel_t;

typedef uint8_t lor_channel_action_magic_t;

static const lor_channel_action_magic_t LOR_CHANNEL_ACTION_MAGIC_MASK8 = 0x30;
static const lor_channel_action_magic_t LOR_CHANNEL_ACTION_MAGIC_MASK16 = 0x10;
static const lor_channel_action_magic_t LOR_CHANNEL_ACTION_MAGIC_CHAIN = 0x50;

lor_channel_action_magic_t lor_get_channel_action_magic(LORChannelType type);

/**
 * Brightness
 */
typedef uint8_t lor_brightness_t;

static const lor_brightness_t LOR_BRIGHTNESS_MIN = 0xF0;
static const lor_brightness_t LOR_BRIGHTNESS_MAX = 0x01;

static const float LOR_BRIGHTNESS_RANGE = 0x01 - 0xF0;

/**
 * Brightness Curves
 */
typedef lor_brightness_t (*lor_brightness_curve_t)(float normal);

/**
 * Durations
 */
typedef uint16_t lor_duration_t;

static const lor_duration_t LOR_DURATION_MAX = 0x8014;
static const lor_duration_t LOR_DURATION_MIN = 0x13EB;

static const float LOR_DURATION_SECONDS_MAX = 25;
static const float LOR_DURATION_SECONDS_MIN = 0.1F;

lor_duration_t lor_duration_of(float seconds);

/**
 * Channel Actions
 */
typedef uint8_t lor_channel_action_t;

static const lor_channel_action_t LOR_ACTION_CHANNEL_ON = 0x01;
static const lor_channel_action_t LOR_ACTION_CHANNEL_SET_BRIGHTNESS = 0x03;
static const lor_channel_action_t LOR_ACTION_CHANNEL_FADE = 0x04;
static const lor_channel_action_t LOR_ACTION_CHANNEL_TWINKLE = 0x06;
static const lor_channel_action_t LOR_ACTION_CHANNEL_SHIMMER = 0x07;

/**
 * Unit Actions
 */
typedef uint16_t lor_unit_action_t;

static const lor_unit_action_t LOR_ACTION_UNIT_OFF = 0x41;

#endif //LIGHTORAMA_PROTOCOL_H
