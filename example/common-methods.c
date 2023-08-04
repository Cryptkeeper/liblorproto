#include <lightorama/lightorama.h>

#include <stdio.h>

// this is where your program would receive a written byte from the library
// and should either insert it into a queue/buffer, or pass it along to
// the serial port for writing to the hardware
static void write(const unsigned char b) {
    printf("%02x\b", b);
}

static void turnOffUnit(const LorUnit unit) {
    // turn off all channels on the specified unit
    lorEncodeUnitEffect(LOR_EFFECT_SET_OFF, NULL, 0, unit, write);
}

static void turnOffAllUnits(void) {
    // turn off all channels on the specified unit
    lorEncodeUnitEffect(LOR_EFFECT_SET_OFF, NULL, 0, LOR_UNIT_ALL, write);
}

static void setChannelBrightness(const LorUnit unit,
                                 const LorChannel channel,
                                 const float brightness) {
    const LorSetIntensityArgs args = {
            // the "vendor" curve is most likely what you want, but you can use
            // others or write your own
            .intensity = LorIntensityCurveVendor(brightness),
    };

    // sets the brightness (0.0-1.0 range) for the specified unit->channel
    lorEncodeChannelEffect(LOR_EFFECT_SET_INTENSITY, &args, sizeof(args),
                           channel, unit, write);
}

static void fadeChannel(const LorUnit unit,
                        const LorChannel channel,
                        const float from,
                        const float to,
                        const float seconds) {
    const LorFadeArgs args = {
            .startIntensity = LorIntensityCurveVendor(from),
            .endIntensity   = LorIntensityCurveVendor(to),
            .duration       = lorSecondsToTime(seconds),
    };

    // starts a fade effect between the two brightness values for the specified
    // unit->channel over the specified period of time (0.1s - 25s range)
    lorEncodeChannelEffect(LOR_EFFECT_FADE, &args, sizeof(args), channel, unit,
                           write);
}

static void variousChannelEffects(const LorUnit unit,
                                  const LorChannel channel) {
    lorEncodeChannelEffect(LOR_EFFECT_TWINKLE, NULL, 0, channel, unit, write);

    lorEncodeChannelEffect(LOR_EFFECT_SHIMMER, NULL, 0, channel, unit, write);
}

static void setMultipleChannelBrightness(const LorUnit unit,
                                         const LorChannelSet channelSet,
                                         const float brightness) {
    const LorSetIntensityArgs args = {
            // the "vendor" curve is most likely what you want, but you can use
            // others or write your own
            .intensity = LorIntensityCurveVendor(brightness),
    };

    // sets the brightness (0.0-1.0 range) for the specified channels on `unit`
    lorEncodeChannelSetEffect(LOR_EFFECT_SET_INTENSITY, &args, sizeof(args),
                              channelSet, unit, write);
}

static void shimmerWhileFading(const LorUnit unit,
                               const LorChannel channel,
                               const float from,
                               const float to,
                               const float seconds) {
    const LorFadeArgs args = {
            .startIntensity = LorIntensityCurveVendor(from),
            .endIntensity   = LorIntensityCurveVendor(to),
            .duration       = lorSecondsToTime(seconds),
    };

    // starts a fade effect between the two brightness values for the specified
    // unit->channel over the specified period of time (0.1s - 25s range)
    // while fading, the shimmer effect is also applied - `LOR_EFFECT_TWINKLE` can also be used
    lorEncodeLayeredChannelEffect(LOR_EFFECT_SHIMMER, LOR_EFFECT_FADE, &args,
                                  sizeof(args), channel, unit, write);
}
