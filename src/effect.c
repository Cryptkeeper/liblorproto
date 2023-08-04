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

#include "lightorama/time.h"

void lorEncodeEffect(const LorEffect effect,
                     const LorChannelFormat format,
                     const LorWriteFn write) {
    write(effect | format);
}

bool lorEncodeEffectArgs(const LorEffect effect,
                         const void *const args,
                         const int argsSize,
                         const LorWriteFn write) {
    switch (effect) {
        case LOR_EFFECT_SET_INTENSITY:
            if (!args || argsSize != sizeof(LorSetIntensityArgs)) return false;

            const LorSetIntensityArgs *const setArgs =
                    (LorSetIntensityArgs *) args;

            write(setArgs->intensity);

            return true;

        case LOR_EFFECT_FADE:
            if (!args || argsSize != sizeof(LorFadeArgs)) return false;

            const LorFadeArgs *const fadeArgs = (LorFadeArgs *) args;

            write(fadeArgs->startIntensity);
            write(fadeArgs->endIntensity);

            lorEncodeTime(fadeArgs->duration, write);

            return true;

        case LOR_EFFECT_PULSE:
            if (!args || argsSize != sizeof(LorPulseArgs)) return false;

            const LorPulseArgs *const pulseArgs = (LorPulseArgs *) args;

            lorEncodeTime(pulseArgs->halfInterval, write);

            return true;

        default:
            return false;
    }
}
