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
#include "lightorama/heartbeat.h"

#include "lightorama/uid.h"

LorResult lorEncodeUnitHeartbeat(const LorUnit unit, const LorWriteFn write) {
    if (write) {
        lorEncodeUnit(unit, write);

        write(0x81);
        write(0x56);
    }

    return LorOK;
}

LorResult lorEncodeHeartbeat(const LorWriteFn write) {
    // 0xFF is a magic protocol value that matches all connected units
    return lorEncodeUnitHeartbeat(0xFF, write);
}
