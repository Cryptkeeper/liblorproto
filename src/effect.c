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

#include "lightorama/intensity.h"
#include "lightorama/time.h"

int lor_write_effect_struct(lor_effect_t effect,
                            const void *effectStruct,
                            lor_uint8_t *b) {
    switch (effect) {
        case LOR_EFFECT_SET_INTENSITY: {
            struct lor_effect_setintensity_t md =
                    *((struct lor_effect_setintensity_t *) effectStruct);
            b[0] = md.intensity;
            return 1;
        }

        case LOR_EFFECT_FADE: {
            struct lor_effect_fade_t md =
                    *((struct lor_effect_fade_t *) effectStruct);
            int n = 0;
            b[n++] = md.startIntensity;
            b[n++] = md.endIntensity;
            n += lor_write_time(md.duration, &b[n]);
            return n;
        }

        case LOR_EFFECT_PULSE: {
            struct lor_effect_pulse_t md =
                    *((struct lor_effect_pulse_t *) effectStruct);
            return lor_write_time(md.halfInterval, b);
        }

        default:
            return 0;
    }
}

int lor_read_effect_struct(lor_effect_t effect,
                           union lor_effect_any_t *effectStruct,
                           const lor_uint8_t *b) {
    switch (effect) {
        case LOR_EFFECT_SET_INTENSITY: {
            struct lor_effect_setintensity_t *md = &effectStruct->setIntensity;
            return lor_read_intensity(&md->intensity, b);
        }

        case LOR_EFFECT_FADE: {
            struct lor_effect_fade_t *md = &effectStruct->fade;
            int n = 0;
            n += lor_read_intensity(&md->startIntensity, b);
            n += lor_read_intensity(&md->endIntensity, &b[n]);
            n += lor_read_time(&md->duration, &b[n]);
            return n;
        }

        case LOR_EFFECT_PULSE: {
            struct lor_effect_pulse_t *md = &effectStruct->pulse;
            return lor_read_time(&md->halfInterval, b);
        }

        default:
            return 0;
    }
}

int lor_is_effect_eq(lor_effect_t effect, const void *aes, const void *bes) {
    switch (effect) {
        case LOR_EFFECT_SET_LIGHTS:
        case LOR_EFFECT_SET_OFF:
            return 1;

        case LOR_EFFECT_SET_INTENSITY: {
            struct lor_effect_setintensity_t amd =
                    *((struct lor_effect_setintensity_t *) aes);
            struct lor_effect_setintensity_t bmd =
                    *((struct lor_effect_setintensity_t *) bes);
            return amd.intensity == bmd.intensity;
        }

        case LOR_EFFECT_FADE: {
            struct lor_effect_fade_t amd = *((struct lor_effect_fade_t *) aes);
            struct lor_effect_fade_t bmd = *((struct lor_effect_fade_t *) bes);
            return amd.startIntensity == bmd.startIntensity &&
                   amd.endIntensity == bmd.endIntensity &&
                   amd.duration == bmd.duration;
        }

        case LOR_EFFECT_PULSE: {
            struct lor_effect_pulse_t amd =
                    *((struct lor_effect_pulse_t *) aes);
            struct lor_effect_pulse_t bmd =
                    *((struct lor_effect_pulse_t *) bes);
            return amd.halfInterval == bmd.halfInterval;
        }

        case LOR_EFFECT_TWINKLE:
        case LOR_EFFECT_SHIMMER:
            return 1;

        default:
            return -1;
    }
}
