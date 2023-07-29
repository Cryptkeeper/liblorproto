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

typedef enum LorEffect {
    LOR_EFFECT_SET_LIGHTS = 0b0001,   // 0x01
    LOR_EFFECT_SET_OFF = 0b0010,      // 0x02
    LOR_EFFECT_SET_INTENSITY = 0b0011,// 0x03
    LOR_EFFECT_FADE = 0b0100,         // 0x04
    LOR_EFFECT_PULSE = 0b0101,        // 0x05
    LOR_EFFECT_TWINKLE = 0b0110,      // 0x06
    LOR_EFFECT_SHIMMER = 0b0111,      // 0x07
} LorEffect;

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

/*
 * `lorWriteEffectArgs` encodes the `args` structure as binary protocol data into
 * `b`, according to the value of `effect`.
 *
 * `args` MUST be a non-NULL when `effect` is `LOR_EFFECT_SET_INTENSITY`,
 * `LOR_EFFECT_FADE` or `LOR_EFFECT_PULSE`. The value of `args` should be a pointer
 * to the corresponding structure for a given `effect` value in the table below.
 * `argsSize` MUST be set to the `sizeof` of the corresponding structure.
 *
 * Effect Argument Structures:
 *
 *  `LOR_EFFECT_SET_INTENSITY`        `LorSetIntensityArgs`
 *  `LOR_EFFECT_FADE`                 `LorFadeArgs`
 *  `LOR_EFFECT_PULSE`                `LorPulseArgs`
 *
 * Any other effect value MUST provide a NULL `args` value with `argsSize` of 0.
 *
 * `lorWriteEffectArgs` encodes the arguments into binary protocol binary data into
 * the provided buffer pointer, `b`.
 *
 * If `b` is NULL, no data is encoded and instead the minimum buffer size for
 * encoding the arguments is returned. This is useful for determining buffer sizes
 * prior to allocating. `bufSize` is ignored in this scenario, but the value
 * should be zero for clarity.
 *
 * Otherwise, `bSize` is checked to ensure `b` has available capacity. If `bSize`
 * is too small, `LorErrOutOfBuffer` is returned and `b` remains unmodified.
 *
 * Return Values:
 *
 *  If successful (>= 0), the return value is the number of bytes written to `b`.
 *  Zero bytes written is normal behavior for some uses and does not indicate an
 *  error.
 *
 *  If `b` is NULL, this represents the minimum value of `bSize` required for encoding
 *  the arguments with a non-NULL `b` buffer.
 *
 * Errors:
 *
 *  LorErrInvalidArg: `args` is NULL when it should not be OR `args` is not NULL
 *                      when it should be OR `argsSize` does not match the expected
 *                      sizeof value according to the table
 *
 *  LorErrOutOfBuffer: the size of the output buffer `b`, according to `bSize`,
 *                       is too small
 *
 */
LorResult lorEncodeEffectArgs(LorEffect effect,
                              const void *args,
                              size_t argsSize,
                              unsigned char *b,
                              size_t bSize);

#endif// LIGHTORAMA_EFFECT_H
