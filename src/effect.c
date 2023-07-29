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

LorResult lorEncodeEffectArgs(const LorEffect effect,
                              const void *const args,
                              const size_t argsSize,
                              unsigned char *const b,
                              const size_t bSize) {
    switch (effect) {
        case LOR_EFFECT_SET_INTENSITY:
            if (args == NULL || argsSize != sizeof(LorSetIntensityArgs))
                return LorErrInvalidArg;

            const LorSetIntensityArgs *const setArgs =
                    (LorSetIntensityArgs *) args;

            if (b != NULL) {
                if (bSize < 1) return LorErrOutOfBuffer;

                b[0] = setArgs->intensity;
            }

            return 1;

        case LOR_EFFECT_FADE:
            if (args == NULL || argsSize != sizeof(LorFadeArgs))
                return LorErrInvalidArg;

            const LorFadeArgs *const fadeArgs = (LorFadeArgs *) args;

            if (b != NULL) {
                if (bSize < 4) return LorErrOutOfBuffer;

                b[0] = fadeArgs->startIntensity;
                b[1] = fadeArgs->endIntensity;

                LorResult err;

                if ((err = lorEncodeTime(fadeArgs->duration, &b[2],
                                         bSize - 2)) < 0)
                    return err;
            }

            return 4;

        case LOR_EFFECT_PULSE:
            if (args == NULL || argsSize != sizeof(LorPulseArgs))
                return LorErrInvalidArg;

            const LorPulseArgs *const pulseArgs = (LorPulseArgs *) args;

            if (b != NULL) {
                if (bSize < 2) return LorErrOutOfBuffer;

                LorResult err;

                if ((err = lorEncodeTime(pulseArgs->halfInterval, &b[0],
                                         bSize - 2)) < 0)
                    return err;
            }

            return 2;

        default:
            if (args != NULL || argsSize != 0) return LorErrInvalidArg;

            return 0;
    }
}
