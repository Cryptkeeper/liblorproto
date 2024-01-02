#include <lorproto/lorproto.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool allocSingleMessageBuffer(LorBuffer *const b) {
    const size_t memSize = 32;

    // allocate an array used by the buffer for storing the encoded data
    // most likely this is a pointer to a single outbound buffer you're appending to
    uint8_t *mem = (uint8_t *) malloc(memSize);

    if (mem == NULL) {
        printf("error: out of memory?\n");

        return false;
    }

    // buffer tracks read/write index handling for bounds checking
    // nothing is allocated internally by LorBuffer, it uses the provided alloc
    *b = lorBufferInit(mem, memSize);

    return true;
}

static void freeBuffer(LorBuffer *const b) {
    // nothing is allocated internally by LorBuffer, it uses the provided alloc
    // this means it doesn't know how to free and the caller (us) must free it
    free(b->buffer);

    b->buffer = NULL;
}

static void turnOffUnit(const LorUnit unit) {
    LorBuffer b;
    if (!allocSingleMessageBuffer(&b)) return;

    // turn off all channels on the specified unit
    lorAppendUnitEffect(&b, LOR_EFFECT_SET_OFF, NULL, unit);

    // TODO: copy and send up to `b.offset` number of bytes starting @ `b.buffer`

    freeBuffer(&b);
}

static void turnOffAllUnits(void) {
    LorBuffer b;
    if (!allocSingleMessageBuffer(&b)) return;

    // turn off all channels on the specified unit
    lorAppendUnitEffect(&b, LOR_EFFECT_SET_OFF, NULL, LOR_UNIT_ALL);

    // TODO: copy and send up to `b.offset` number of bytes starting @ `b.buffer`

    freeBuffer(&b);
}

static void setChannelBrightness(const LorUnit unit,
                                 const LorChannel channel,
                                 const float brightness) {
    LorBuffer b;
    if (!allocSingleMessageBuffer(&b)) return;

    const union LorEffectArgs args = {
            .setIntensity = {
                    // the "vendor" curve is most likely what you want, but you can use
                    // others or write your own
                    .intensity = LorIntensityCurveVendor(brightness),
            }};

    // sets the brightness (0.0-1.0 range) for the specified unit->channel
    lorAppendChannelEffect(&b, LOR_EFFECT_SET_INTENSITY, &args, channel, unit);

    // TODO: copy and send up to `b.offset` number of bytes starting @ `b.buffer`

    freeBuffer(&b);
}

static void fadeChannel(const LorUnit unit,
                        const LorChannel channel,
                        const float from,
                        const float to,
                        const int deciseconds) {
    LorBuffer b;
    if (!allocSingleMessageBuffer(&b)) return;

    const union LorEffectArgs args = {
            .fade = {
                    .startIntensity = LorIntensityCurveVendor(from),
                    .endIntensity   = LorIntensityCurveVendor(to),
                    .deciseconds    = deciseconds,
            }};

    // starts a fade effect between the two brightness values for the specified
    // unit->channel over the specified period of time (0.1s - 25s range)
    lorAppendChannelEffect(&b, LOR_EFFECT_FADE, &args, channel, unit);

    // TODO: copy and send up to `b.offset` number of bytes starting @ `b.buffer`

    freeBuffer(&b);
}

static void variousChannelEffects(const LorUnit unit,
                                  const LorChannel channel) {
    {// "twinkle" protocol effect
        LorBuffer b;

        if (allocSingleMessageBuffer(&b)) {
            lorAppendChannelEffect(&b, LOR_EFFECT_TWINKLE, NULL, channel, unit);

            // TODO: copy and send up to `b.offset` number of bytes starting @ `b.buffer`

            freeBuffer(&b);
        }
    }

    {// "shimmer" protocol effect
        LorBuffer b;

        if (allocSingleMessageBuffer(&b)) {
            lorAppendChannelEffect(&b, LOR_EFFECT_SHIMMER, NULL, channel, unit);

            // TODO: copy and send up to `b.offset` number of bytes starting @ `b.buffer`

            freeBuffer(&b);
        }
    }
}

static void setMultipleChannelBrightness(const LorUnit unit,
                                         const LorChannelSet channelSet,
                                         const float brightness) {
    LorBuffer b;
    if (!allocSingleMessageBuffer(&b)) return;

    const union LorEffectArgs args = {
            .setIntensity = {
                    // the "vendor" curve is most likely what you want, but you can use
                    // others or write your own
                    .intensity = LorIntensityCurveVendor(brightness),
            }};

    // sets the brightness (0.0-1.0 range) for the specified channels on `unit`
    lorAppendChannelSetEffect(&b, LOR_EFFECT_SET_INTENSITY, &args, channelSet,
                              unit);

    // TODO: copy and send up to `b.offset` number of bytes starting @ `b.buffer`

    freeBuffer(&b);
}

static void shimmerWhileFading(const LorUnit unit,
                               const LorChannel channel,
                               const float from,
                               const float to,
                               const int deciseconds) {
    LorBuffer b;
    if (!allocSingleMessageBuffer(&b)) return;

    const union LorEffectArgs args = {
            .fade = {
                    .startIntensity = LorIntensityCurveVendor(from),
                    .endIntensity   = LorIntensityCurveVendor(to),
                    .deciseconds    = deciseconds,
            }};

    // starts a fade effect between the two brightness values for the specified
    // unit->channel over the specified period of time (0.1s - 25s range)
    // while fading, the shimmer effect is also applied - `LOR_EFFECT_TWINKLE` can also be used
    lorAppendLayeredChannelEffect(&b, LOR_EFFECT_SHIMMER, LOR_EFFECT_FADE,
                                  &args, channel, unit);

    // TODO: copy and send up to `b.offset` number of bytes starting @ `b.buffer`

    freeBuffer(&b);
}
