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
#include "lightorama/easy.h"

#include "lightorama/effect.h"
#include "lightorama/uid.h"

void lorEncodeChannelEffect(LorEffect effect,
                            const LorEffectArgs *const args,
                            LorChannel channel,
                            LorUnit unit,
                            LorWriteFn write) {
    lorEncodeUnit(unit, write);
    lorEncodeEffect(effect, LOR_FORMAT_SINGLE, write);
    lorEncodeEffectArgs(effect, args, write);
    lorEncodeChannel(channel, write);
}

void lorEncodeLayeredChannelEffect(LorEffect primaryEffect,
                                   LorEffect secondaryEffect,
                                   const LorEffectArgs *const args,
                                   LorChannel channel,
                                   LorUnit unit,
                                   LorWriteFn write) {
    lorEncodeUnit(unit, write);
    lorEncodeEffect(primaryEffect, LOR_FORMAT_SINGLE, write);
    lorEncodeChannel2(channel, LOR_ALIGN_16, write);
    lorEncodeEffect(secondaryEffect, LOR_FORMAT_SINGLE, write);
    lorEncodeEffectArgs(secondaryEffect, args, write);
}

void lorEncodeChannelSetEffect(LorEffect effect,
                               const LorEffectArgs *const args,
                               LorChannelSet channelSet,
                               LorUnit unit,
                               LorWriteFn write) {
    const LorChannelFormat format = lorGetChannelSetFormat(channelSet);

    lorEncodeUnit(unit, write);
    lorEncodeEffect(effect, format, write);
    lorEncodeEffectArgs(effect, args, write);
    lorEncodeChannelSet(channelSet, write);
}

void lorEncodeUnitEffect(LorEffect effect,
                         const LorEffectArgs *const args,
                         LorUnit unit,
                         LorWriteFn write) {
    lorEncodeUnit(unit, write);
    lorEncodeEffect(effect, LOR_FORMAT_UNIT, write);
    lorEncodeEffectArgs(effect, args, write);
}