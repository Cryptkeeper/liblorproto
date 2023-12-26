/*
 * MIT License
 *
 * Copyright (c) 2023 Nick Krecklow
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
#include "lightorama/coretypes.h"

#undef NDEBUG
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void test_buffer_init_local(void) {
    uint8_t fakeStack[4] = {0};

    LorBuffer b = lorBufferInit(&fakeStack[0], 4);

    assert(b.buffer == &fakeStack[0]);
    assert(b.size == 4);
    assert(b.offset == 0);
}

static void test_buffer_append_local(void) {
    uint8_t fakeStack[4] = {0};

    LorBuffer b = lorBufferInit(&fakeStack[0], 4);

    assert(b.buffer == &fakeStack[0]);
    assert(b.size == 4);
    assert(b.offset == 0);

    assert(lorBufferRemaining(b) == b.size);

    // test write behaves as expected
    lorAppendU8(&b, 11);

    assert(b.offset == 1);
    assert(lorBufferRemaining(b) == b.size - 1);

    lorAppendU8(&b, 22);

    assert(b.offset == 2);
    assert(lorBufferRemaining(b) == b.size - 2);

    lorAppendU8(&b, 33);

    assert(b.offset == 3);
    assert(lorBufferRemaining(b) == b.size - 3);

    lorAppendU8(&b, 44);

    assert(b.offset == 4);
    assert(lorBufferRemaining(b) == b.size - 4);

    // validate write contents and order
    assert(b.buffer[0] == 11);
    assert(b.buffer[1] == 22);
    assert(b.buffer[2] == 33);
    assert(b.buffer[3] == 44);

    // reset write head and try appending again
    // ensures buffer can be rewritten as expected
    b.offset = 0;

    lorAppendU8(&b, 55);
    lorAppendU8(&b, 66);
    lorAppendU8(&b, 77);
    lorAppendU8(&b, 88);

    assert(lorBufferRemaining(b) == 0);
    assert(b.offset == 4);

    assert(b.buffer[0] == 55);
    assert(b.buffer[1] == 66);
    assert(b.buffer[2] == 77);
    assert(b.buffer[3] == 88);
}

static void test_buffer_init_malloc(void) {
    const size_t memSize = 32;

    uint8_t *alloc = malloc(memSize);

    // fail the test if out of memory, no nice way to handle this
    assert(alloc != NULL);

    LorBuffer b = lorBufferInit(alloc, memSize);

    assert(b.buffer == alloc);
    assert(b.size == memSize);
    assert(b.offset == 0);

    free(alloc);
}

int main(__attribute__((unused)) int argc,
         __attribute__((unused)) char **argv) {
    test_buffer_init_local();
    test_buffer_append_local();

    test_buffer_init_malloc();

    return 0;
}
