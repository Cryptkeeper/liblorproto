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
#ifndef LIGHTORAMA_EFFECT_H
#define LIGHTORAMA_EFFECT_H

#include "coretypes.h"
#include "uid.h"

typedef enum LorEffect {
    LOR_EFFECT_SET_LIGHTS    = 0x01, /* 0b0001 */
    LOR_EFFECT_SET_OFF       = 0x02, /* 0b0010 */
    LOR_EFFECT_SET_INTENSITY = 0x03, /* 0b0011 */
    LOR_EFFECT_FADE          = 0x04, /* 0b0100 */
    LOR_EFFECT_PULSE         = 0x05, /* 0b0101 */
    LOR_EFFECT_TWINKLE       = 0x06, /* 0b0110 */
    LOR_EFFECT_SHIMMER       = 0x07, /* 0b0111 */
} LorEffect;

void lorEncodeEffect(LorEffect effect,
                     LorChannelFormat format,
                     LorWriteFn write);

typedef struct LorSetIntensityArgs {
    LorIntensity intensity;
} LorSetIntensityArgs;

typedef struct LorFadeArgs {
    LorIntensity startIntensity;
    LorIntensity endIntensity;
    LorTime duration;
} LorFadeArgs;

typedef struct LorPulseArgs {
    LorTime halfInterval;
} LorPulseArgs;

typedef union LorAnyArgs {
    LorSetIntensityArgs setIntensity;
    LorFadeArgs fade;
    LorPulseArgs pulse;
} LorEffectArgs;

void lorEncodeEffectArgs(LorEffect effect,
                         const LorEffectArgs *args,
                         LorWriteFn write);

#endif// LIGHTORAMA_EFFECT_H
