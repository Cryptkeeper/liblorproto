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
#ifndef LIGHTORAMA_CORETYPES_H
#define LIGHTORAMA_CORETYPES_H

#include <stdint.h>

typedef uint8_t LorUnit;
typedef uint8_t LorIntensity;
typedef uint8_t LorChannel;

typedef struct LorChannelSet {
    uint8_t offset;      /* 6-bit unsigned int, max value of 64 */
    uint16_t channelBits;/* 16-bit bitset of channels to apply changes to */
} LorChannelSet;

typedef struct LorBuffer {
    uint8_t *buffer;/* pointer to backing alloc for reading/writing bytes */
    uint32_t size;  /* buffer memory allocation size for write bounds checks */
    uint32_t offset;/* relative position of write head, # of bytes written */
} LorBuffer;

LorBuffer lorBufferInit(uint8_t *mem, uint32_t size);

uint32_t lorBufferRemaining(LorBuffer b);

void lorAppendU8(LorBuffer *b, uint8_t v);

#endif// LIGHTORAMA_CORETYPES_H
