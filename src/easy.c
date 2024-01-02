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
#include "lorproto/easy.h"

#include "lorproto/effect.h"
#include "lorproto/uid.h"

void lorAppendChannelEffect(LorBuffer *const b,
                            const LorEffect effect,
                            const union LorEffectArgs *const args,
                            const LorChannel channel,
                            const LorUnit unit) {
    lorAppendU8(b, 0);
    lorAppendUnit(b, unit);
    lorAppendEffect(b, effect, LOR_FORMAT_SINGLE);
    lorAppendEffectArgs(b, effect, args);
    lorAppendChannel(b, channel);
    lorAppendU8(b, 0);
}

void lorAppendLayeredChannelEffect(LorBuffer *const b,
                                   const LorEffect primaryEffect,
                                   const LorEffect secondaryEffect,
                                   const union LorEffectArgs *const args,
                                   const LorChannel channel,
                                   const LorUnit unit) {
    lorAppendU8(b, 0);
    lorAppendUnit(b, unit);
    lorAppendEffect(b, primaryEffect, LOR_FORMAT_SINGLE);
    lorAppendAlignedChannel(b, channel, LOR_ALIGN_16);
    lorAppendEffect(b, secondaryEffect, LOR_FORMAT_SINGLE);
    lorAppendEffectArgs(b, secondaryEffect, args);
    lorAppendU8(b, 0);
}

void lorAppendChannelSetEffect(LorBuffer *const b,
                               const LorEffect effect,
                               const union LorEffectArgs *const args,
                               const LorChannelSet channelSet,
                               const LorUnit unit) {
    const LorChannelFormat format = lorGetChannelSetFormat(channelSet);

    lorAppendU8(b, 0);
    lorAppendUnit(b, unit);
    lorAppendEffect(b, effect, format);
    lorAppendEffectArgs(b, effect, args);
    lorAppendChannelSet(b, channelSet);
    lorAppendU8(b, 0);
}

void lorAppendUnitEffect(LorBuffer *const b,
                         const LorEffect effect,
                         const union LorEffectArgs *const args,
                         const LorUnit unit) {
    lorAppendU8(b, 0);
    lorAppendUnit(b, unit);
    lorAppendEffect(b, effect, LOR_FORMAT_UNIT);
    lorAppendEffectArgs(b, effect, args);
    lorAppendU8(b, 0);
}
