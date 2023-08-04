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

bool lorEncodeChannelEffect(LorEffect effect,
                            const void *args,
                            int argsSize,
                            LorChannel channel,
                            LorUnit unit,
                            LorWriteFn write) {
    lorEncodeUnit(unit, write);
    lorEncodeEffect(effect, LOR_FORMAT_SINGLE, write);

    if (args && !lorEncodeEffectArgs(effect, args, argsSize, write))
        return false;

    return lorEncodeChannel(channel, write);
}

bool lorEncodeLayeredChannelEffect(LorEffect primaryEffect,
                                   LorEffect secondaryEffect,
                                   const void *args,
                                   int argsSize,
                                   LorChannel channel,
                                   LorUnit unit,
                                   LorWriteFn write) {
    lorEncodeUnit(unit, write);
    lorEncodeEffect(primaryEffect, LOR_FORMAT_SINGLE, write);

    if (!lorEncodeChannel2(channel, 16, write)) return false;

    lorEncodeEffect(secondaryEffect, LOR_FORMAT_SINGLE, write);

    if (args && !lorEncodeEffectArgs(secondaryEffect, args, argsSize, write))
        return false;

    return true;
}

bool lorEncodeChannelSetEffect(LorEffect effect,
                               const void *args,
                               int argsSize,
                               LorChannelSet channelSet,
                               LorUnit unit,
                               LorWriteFn write) {
    const LorChannelFormat format = lorGetChannelSetFormat(channelSet);

    lorEncodeUnit(unit, write);
    lorEncodeEffect(effect, format, write);

    if (args && !lorEncodeEffectArgs(effect, args, argsSize, write))
        return false;

    return lorEncodeChannelSet(channelSet, write);
}

bool lorEncodeUnitEffect(LorEffect effect,
                         const void *args,
                         int argsSize,
                         LorUnit unit,
                         LorWriteFn write) {
    lorEncodeUnit(unit, write);
    lorEncodeEffect(effect, LOR_FORMAT_UNIT, write);

    if (args && !lorEncodeEffectArgs(effect, args, argsSize, write))
        return false;

    return true;
}