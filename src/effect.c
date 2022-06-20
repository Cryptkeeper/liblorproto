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
#include "../include/effect.h"

int lor_write_effect_struct(lor_effect_t effect, const void *effectStruct, uint8_t *b) {
  switch (effect) {
  case LOR_EFFECT_SET_INTENSITY: {
    struct lor_effect_setintensity_t md = *((struct lor_effect_setintensity_t *)effectStruct);
    b[0] = md.intensity;
    return 1;
  }

  case LOR_EFFECT_FADE: {
    struct lor_effect_fade_t md = *((struct lor_effect_fade_t *)effectStruct);
    int n = 0;
    b[n++] = md.startIntensity;
    b[n++] = md.endIntensity;
    n += lor_write_time(md.duration, &b[n]);
    return n;
  }

  case LOR_EFFECT_PULSE: {
    struct lor_effect_pulse_t md = *((struct lor_effect_pulse_t *)effectStruct);
    return lor_write_time(md.halfInterval, b);
  }

  default:
    return 0;
  }
}

int lor_read_effect_struct(lor_effect_t effect,
                           union lor_effect_any_t *effectStruct,
                           const uint8_t *b) {
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

int lor_write_channel_effect(lor_effect_t effect,
                             const void *effectStruct,
                             lor_channel_t channel,
                             lor_unit_t unit,
                             uint8_t *b) {
  int n = 0;
  n += lor_write_unit(unit, &b[n]);
  b[n++] = effect;
  n += lor_write_effect_struct(effect, effectStruct, &b[n]);
  n += lor_write_channel(channel, &b[n]);
  return n;
}

static uint8_t lor_get_channelset_offset_opts(lor_channelset_t channelset) {
  if (channelset.offset > 0) {
    return LOR_OFFSET_OPT_MULTIPART;
  } else {
    const uint8_t bank0 = (channelset.channels & 0x00FF);
    const uint8_t bank1 = (channelset.channels & 0xFF00) >> 8;

    if (bank0 > 0x00 && bank1 > 0x00) {
      return LOR_OFFSET_OPT_16;
    } else if (bank0 > 0) {
      return LOR_OFFSET_OPT_8L;
    } else /* if (bank1 > 0) */ {
      return LOR_OFFSET_OPT_8H;
    }
  }
}

int lor_write_channelset_effect(lor_effect_t effect,
                                const void *effectStruct,
                                lor_channelset_t channelset,
                                lor_unit_t unit,
                                uint8_t *b) {
  int n = 0;
  n += lor_write_unit(unit, &b[n]);
  b[n++] = effect | lor_get_channelset_offset_opts(channelset);
  n += lor_write_effect_struct(effect, effectStruct, &b[n]);
  n += lor_write_channelset(channelset, &b[n]);
  return n;
}

int lor_write_unit_effect(lor_effect_t effect,
                          const void *effectStruct,
                          lor_unit_t unit,
                          uint8_t *b) {
  int n = 0;
  n += lor_write_unit(unit, &b[n]);
  b[n++] = effect | LOR_OFFSET_OPT_UNIT;
  n += lor_write_effect_struct(effect, effectStruct, &b[n]);
  return n;
}
