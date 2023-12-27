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
#include "lightorama/effect.h"

#include <assert.h>

#include "lightorama/intensity.h"
#include "lightorama/time.h"

void lorAppendEffect(LorBuffer *const b,
                     const LorEffect effect,
                     const LorChannelFormat format) {
    lorAppendU8(b, effect | format);
}

void lorAppendEffectArgs(LorBuffer *const b,
                         const LorEffect effect,
                         const union LorEffectArgs *const args) {
    switch (effect) {
        case LOR_EFFECT_SET_INTENSITY:
            assert(args);
            lorAppendIntensity(b, args->setIntensity.intensity);
            break;

        case LOR_EFFECT_FADE:
            assert(args);
            lorAppendIntensity(b, args->fade.startIntensity);
            lorAppendIntensity(b, args->fade.endIntensity);
            lorAppendTime(b, args->fade.duration);
            break;

        case LOR_EFFECT_PULSE:
            assert(args);
            lorAppendTime(b, args->pulse.halfInterval);
            break;

        case LOR_EFFECT_SET_DMX_INTENSITY:
            assert(args);
            lorAppendU8(b, args->setDMXIntensity.intensity);
            break;

        default:
            assert(!args);
            break;
    }
}
