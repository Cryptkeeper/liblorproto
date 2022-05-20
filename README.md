# liblightorama

C90 library implementing
the [Light-O-Rama (LOR) communication protocol](https://github.com/Cryptkeeper/lightorama-protocol) alongside related
APIs, aiming to reduce usage of magic values and competing encoding & helper methods.

Usage of liblightorama assumes existing familiarity with
the [LOR protocol](https://github.com/Cryptkeeper/lightorama-protocol).

## Project Structure

| Header                                          | Purpose                                                                                                                           |
|-------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------|
| [`lightorama/effect.h`](include/effect.h)       | Type definitions for effect related data structures and associated encoding functions                                             |
| [`lightorama/heartbeat.h`](include/heartbeat.h) | Heartbeat encoding functions and associated named helper constants                                                                |
| [`lightorama/intensity.h`](include/intensity.h) | Type definitions for intensity, named helper constants, and brightness curve functions for encoding/decoding normalized values    |
| [`lightorama/model.h`](include/model.h)         | Automatically generated code representation of [LOR_DeviceFile.txt](http://www1.lightorama.com/downloads/LOR_DeviceFile.txt)      |
| [`lightorama/platform.h`](include/platform.h)   | Override library type usages with platform-specific definitions                                                                   |
| [`lightorama/time.h`](include/time.h)           | Type definitions for time durations, named helper constants, and functions for encoding/decoding normalized values                |
| [`lightorama/uid.h`](include/uid.h)             | Type definitions, named helper constants, and encoding/decoding functions for any referenceable addresses (unit IDs, channel IDs) |
| [`lightorama/version.h`](include/version.h)     | Named constants of liblightorama version information                                                                              |

You may optionally use `lightorama/lightorama.h` to automatically include all library header files.

liblightorama strives to be _generally_ portable (typically for use in microcontrollers) and limits its usage of the
standard library, using only the `stdint.h` header for fixed sized int definitions, `stddef.h` for `size_t`, and with
minimal usage of `float`. Usage of `size_t` may be overridden by defining `LOR_PLATFORM_TYPE_SIZE` and including a
`lor_size_t` typedef to an (optionally unsigned) int size of your choice; _prior_ to including `lightorama.h` or any
header that includes `platform.h`.

## Installation

liblightorama uses [CMake](https://cmake.org/) for building and packaging the library.

1. Generate Makefiles using `cmake .`
2. If outdated or missing, use `sh tools/generate_model_h.sh` to generate an updated `include/model.h`
3. Compile the library using `make`
4. Optionally install the headers and compiled archive using `make install`
5. Include your selected headers, or include all headers using `lightorama/lightorama.h`

If optionally installed, `install_manifest.txt` will be created, containing the installed file paths for easy removal.

## Referencing Channels

The LOR protocol has four distinct ways to reference channels. Each becomes increasingly complex, but at the benefit of
far more channel control options—dramatically reducing bandwidth use compared to individual channels.

1. Using a single channel ID.
2. Using 1-2 "channel bank" bit masks for selecting up to 16 sequential channels. A single channel bank is an 8 bit
   mask. Each "channel group" (16 channels)
   therefor contains two channel banks, named "high" (channels 1-8, first channel bank)
   and "low" (channels 9-16, second channel bank). These seemingly backwards names come from their bit order when
   encoded into the LOR protocol, with the "high" channel bank becoming the highest order bits and vice versa.
3. Using the previous mode with a channel group offset, which offsets the channel group mask by a multiple of 16.
4. Using a single unit ID to automatically select all its channels (rarely usable).

```
                 High Channel Bank (1-8)   Low Channel Bank (9-16)
                 ┌───────────────────────┬───────────────────────┐
                 ▼                       │                       ▼
                 ┌──┬──┬──┬──┬──┬──┬──┬──┼──┬──┬──┬──┬──┬──┬──┬──┐
                 │01│02│03│04│05│06│07│08│09│10│11│12│13│14│15│16│
Channels 01-16:  └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
                 Channel Group Offset 0

                 ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
Channels 17-32:  │01│02│03│04│05│06│07│08│09│10│11│12│13│14│15│16│
                 └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
                 Channel Group Offset 1

                 ┌──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┬──┐
Channels 33-48:  │01│02│03│04│05│06│07│08│09│10│11│12│13│14│15│16│
                 └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
                 Channel Group Offset 2
```

Within liblightorama a single channel ID is represented by the `lor_channel_t` type. Keep in mind that channels within
the LOR protocol start at 0, not 1. A `lor_channelset_t` can be used to reference a single channel, but that is likely a
waste of bandwidth and isn't recommended.

For using channel groups and channel banks, the `lor_channelset_t` type contains a `channels` field for defining the
channel banks and an optional `offset` value for providing a channel group offset. liblightorama's `lor_channelset_t`
encoding logic will apply a few protocol optimizations that can help minimize memory usage when referencing a single
channel bank.

Depending on your type (`lor_channel_t`, `lor_channelset_t`, `lor_unit_t`) you will use different functions when
encoding/decoding (e.g. `lor_write_channel_effect` vs `lor_write_channelset_effect` vs `lor_write_unit_effect`). A
missing function signature for your type may mean it is unsupported by the protocol.

### Channel Set Code Examples

```C
lor_channelset_t first_8_channels = { 
        .channels = 0xFF00, // set first 8 high bits
        .offset = 0, // channel group 0
};

lor_channelset_t channels_96_to_112 = {
        .channels = 0xFFFF, // all 16 channels
        .offset = 5, // start at channel 96 (96/16 == 6 - 1 [because group #1 is 0] == 5)
};

lor_channelset_t only_channel_32 = {
        .channels = 0x1, // only set lowest order bit (channel 16)
        .offset = 1, // offset channels by +16
};
```

## Usage Examples

### Turn Off Unit

Turn off all channels on unit `0x01`.

_Tip:_ Changing `unit_id` to `LOR_UNIT_ALL` would turn off all data on ALL units.

```C
#include <lightorama/lightorama.h>

static void turn_off_all_channels(lor_unit_t unit_id) {
    uint8_t msg[128]; // allocate a small data buffer
    
    lor_size_t written = lor_write_unit_effect(LOR_EFFECT_SET_OFF, NULL, unit_id, msg);
    
    // ...write msg buffer to destination...
}

// turn off all channels on unit 1
turn_off_all_channels(0x01);
```

### Set Channel Brightness

Set channel 8 (`0x07`) on unit `0x01` to 50% brightness. This uses the `lor_intensity_curve_linear` brightness curve,
but you can use other brightness curves (or write your own).

In this example, the `LOR_EFFECT_SET_INTENSITY` effect requires a pointer to a
struct (`struct lor_effect_setintensity_t`) which contains metadata used to control the effect. Not all effects require
a metadata structure, and may simply instead pass `NULL`. See [`include/effect.h`](include/effect.h) for additional data
structures definitions.

```C
#include <lightorama/lightorama.h>

static void set_channel_half_brightness(lor_unit_t unit_id, lor_channel_t channel_id) {
    const lor_intensity_t half_intensity = lor_intensity_curve_linear(0.5F);
    
    struct lor_effect_setintensity_t effect;
    effect.intensity = half_intensity;
    
    uint8_t msg[128]; // allocate a small data buffer
    
    lor_size_t written = lor_write_channel_effect(LOR_EFFECT_SET_INTENSITY, &effect, channel_id, unit_id, msg);
    
    // ...write msg buffer to destination...
}

// Set channel 8 (0x07) on unit 0x01 to 50% brightness
set_channel_half_brightness(0x01, 0x07);
```

### Channel Sets & Fading

Fade channels 1-4 from 0% to 100% brightness in 5 seconds on unit `0x01`. This uses the `lor_intensity_curve_squared`
brightness curve to demonstrate alternate brightness curve functionality.

```C
#include <lightorama/lightorama.h>

static void fade_channels(lor_unit_t unit_id, lor_intensity_t from, lor_intensity_t to, float seconds) {
    lor_channel_t channelset = {
            .channels = 0xF000, // only select channels 1-4
            .offset = 0, // starts at channel group offset 0
    }
    
    struct lor_effect_fade_t effect;
    effect.startIntensity = from;
    effect.endIntensity = to;
    effect.duration = lor_seconds_to_time(seconds);
    
    uint8_t msg[128]; // allocate a small data buffer
    
    lor_size_t written = lor_write_channelset_effect(LOR_EFFECT_FADE, &effect, channelset, unit_id, msg);
    
    // ...write msg buffer to destination...
}

fade_channels(0x01, LOR_INTENSITY_MIN, LOR_INTENSITY_MAX, 5.0F);
```

## Library Structure

### Memory Allocations

liblightorama does not allocate any memory internally, and requires a `uint8_t *` be passed to functions for writing to
memory. All write related functions return a `lor_size_t` value (customizable typedef for `size_t` or optimized
equivalent) indicating the length of the data written in bytes. Please track this value alongside the memory allocation
size to ensure write calls do not overflow.

To help size your buffers, consider that while not absolute rule and potentially volatile, even the largest write call
will not write more than 32 bytes.

### Brightness Curves

Brightness curves are responsible for converting a normalized brightness value [0, 1] to
a [LOR protocol brightness value](https://github.com/Cryptkeeper/lightorama-protocol#brightness). The purpose of
different brightness curves is to easily modify the visual appearance of brightness changes without redefining the
underlying normalized values.

#### Pre-implemented Brightness Curves

| Function | Description |
| --- | --- |
| `lor_intensity_curve_linear` | Encodes the normalized value as its direct LOR protocol equivalent value without changes |
| `lor_intensity_curve_squared` | A smoothed version of `lor_intensity_curve_linear` designed to emphasis changes in brightness values |

#### Custom Brightness Curves

Any brightness curve may be implemented assuming it adheres to the `lor_intensity_curve_t` function signature:
`lor_intensity_t (*lor_intensity_curve_t)(float normal)`

Each brightness curve is designed to return a `lor_intensity_t` object representing the LOR protocol equivalent value of
the normalized input, as adapted by the curve. Several consts have been defined
within [`intensity.h`](include/intensity.h) to avoid using magic numbers in your implementations.
See [`intensity.c`](src/intensity.c) for implementation examples.

## Compatibility

liblightorama is implemented following [this documentation](https://github.com/Cryptkeeper/lightorama-protocol), which
was built through reverse engineering efforts. As such, liblightorama has limited compatibility testing and may not work
with your hardware. The current library functionality offerings have been tested with the `LOR1602Wg3` & `CTB16PCg3`
hardware models.

## License

See [LICENSE](LICENSE).
