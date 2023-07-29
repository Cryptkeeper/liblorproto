# liblightorama

C90 library implementing
the [Light-O-Rama (LOR) communication protocol](https://github.com/Cryptkeeper/lightorama-protocol) alongside related
APIs, aiming to reduce usage of magic values and competing encoding & helper methods.

Usage of liblightorama assumes existing familiarity with
the [LOR protocol](https://github.com/Cryptkeeper/lightorama-protocol).

## Project Structure

| Header                                          | Purpose                                                                                                                                                                             |
|-------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [`lightorama/coretypes.h`](include/coretypes.h) | Core typedef definitions and fixed size ints used by the library, likely auto included by another header                                                                            |
| [`lightorama/easy.h`](include/easy.h)           | Ready to go functions for encoding predefined effect structures to a given unit/channel/channel set                                                                                 |
| [`lightorama/effect.h`](include/effect.h)       | Type definitions for effect related data structures and associated encoding functions                                                                                               |
| [`lightorama/heartbeat.h`](include/heartbeat.h) | Heartbeat encoding functions and associated named helper constants                                                                                                                  |
| [`lightorama/intensity.h`](include/intensity.h) | Type definitions for intensity, named helper constants, and brightness curve functions for encoding/decoding normalized values                                                      |
| [`lightorama/model.h`](include/model.h)         | Automatically generated code representation of [LOR_DeviceFile.txt](http://www1.lightorama.com/downloads/LOR_DeviceFile.txt), includes helper functions for referencing model names |
| [`lightorama/time.h`](include/time.h)           | Type definitions for time durations, named helper constants, and functions for encoding/decoding normalized values                                                                  |
| [`lightorama/uid.h`](include/uid.h)             | Type definitions, named helper constants, and encoding/decoding functions for any referenceable addresses (unit IDs, channel IDs)                                                   |
| [`lightorama/version.h`](include/version.h)     | Named constants of liblightorama version information                                                                                                                                |

You may optionally use `lightorama/lightorama.h` to automatically include all library header files (except `model.h` due
to its strings, please include as needed).

liblightorama strives to be fairly portable (typically for use in microcontrollers) and requires no external
dependencies or headers. The few core data types used by liblightorama (e.g. fixed size ints) are defined
in [`include/coretypes.h`](include/coretypes.h). Limited use of floating-point arithmetic is present in
the [`intensity.c`](src/intensity.c) math functions.

## Installation

liblightorama uses [CMake](https://cmake.org/) for building and packaging the library.

1. Generate Makefiles using `cmake .`
2. (Optional) If outdated, use `python3 tools/generate_model_files.py` to generate updated `model.h`
   & `model.c` code files
3. Compile the library using `make`
4. Optionally install the headers and compiled archive using `make install`
5. Include your selected headers, or include all headers using `lightorama/lightorama.h`

If optionally installed, `install_manifest.txt` will be created, containing the installed file paths for easy removal.

## Packet Structure

The majority of data is lighting control packets. Each follows a basic structure, prefixed by the target unit ID, and
followed by a table directory and its corresponding data entry rows, in matching order. The corresponding data
structures used by liblightorama for each field are shown in the diagram below.

```
Light Control Packet
┌─────────────────┐
│Unit             ├─► lor_unit_t
├────────┬────────┤
│Effect  │Channel ├─► lor_effect_t (4 bits)
├────────┴────────┤   lor_uint8_t (4 bits)
│Variable length  │
│effect struct    ├─► struct lor_effect_*_t
│                 │   union lor_effect_any_t
│[0,4] bytes      │
├─────────────────┤
│Variable length  │
│channel struct   ├─► lor_channelset_t
│                 │   lor_channel_t
│[1,2] bytes      │
└─────────────────┘
```

### Implementation Notes

* Additional packet structures exist for remote management/firmware update/system configuration feature sets, but are
  entirely undocumented and more time-consuming to explore. As a result, liblightorama focuses purely on lighting
  control packets.
* A `union lor_effect_any_t` type has been provided to reduce code complexity when deserializing effect data structures.
  It is solely a union of the other individually defined effect data structures.

## Referencing Channels

The LOR protocol has four distinct ways to reference channels. Each becomes increasingly complex, but at the benefit of
far more channel control options—dramatically reducing bandwidth use compared to individual channels.

1. Using a single channel ID.
2. Using 1-2 "channel bank" bit masks for selecting up to 16 sequential channels. A single channel bank is an 8 bit
   mask. Each "channel group" (16 channels)
   therefore contains two channel banks, named "high" (channels 1-8, first channel bank)
   and "low" (channels 9-16, second channel bank). These seemingly backwards names come from their bit order when
   encoded into the LOR protocol, with the "high" channel bank becoming the highest order bits and vice versa.
3. Using the previous mode with a channel group offset, which offsets the channel bank masks by a multiple of 16 (the
   size of a channel group).
4. Using a single unit ID to automatically select all its channels (rarely usable).

```
                 Channel Group
                 ┌───────────────────────────────────────────────┐
                 ▼                                               ▼
                 High Channel Bank                Low Channel Bank
                 ┌───────────────────────┬───────────────────────┐
                 ▼                       │                       ▼
                 ┌──┬──┬──┬──┬──┬──┬──┬──┼──┬──┬──┬──┬──┬──┬──┬──┐
Channels 01-16:  │01│02│03│04│05│06│07│08│09│10│11│12│13│14│15│16│
                 └──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┴──┘
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
    lor_uint8_t msg[128]; // allocate a small data buffer
    
    int written = lor_write_unit_effect(LOR_EFFECT_SET_OFF, NULL, unit_id, msg);
    
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
    
    lor_uint8_t msg[128]; // allocate a small data buffer
    
    int written = lor_write_channel_effect(LOR_EFFECT_SET_INTENSITY, &effect, channel_id, unit_id, msg);
    
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
    
    lor_uint8_t msg[128]; // allocate a small data buffer
    
    int written = lor_write_channelset_effect(LOR_EFFECT_FADE, &effect, channelset, unit_id, msg);
    
    // ...write msg buffer to destination...
}

fade_channels(0x01, LOR_INTENSITY_MIN, LOR_INTENSITY_MAX, 5.0F);
```

### Complex Effects

Complex effects allow the unit to apply two effects simultaneously to a single channel (with limited options). The "
primary" effect, sent first, must be stateless — i.e. no `effectStruct` option — which reduces your options
to `LOR_EFFECT_TWINKLE` and `LOR_EFFECT_SHIMMER`. The "secondary" effect is followed by its respective metadata struct,
restricting it to supporting `LOR_EFFECT_SET_LIGHTS`, `LOR_EFFECT_FADE`, or `LOR_EFFECT_PULSE`.

See the [Complex Effects](#1-complex-effects) section of [Library Structure](#library-structure) for more information.

```C
#include <lightorama/lightorama.h>

static void fade_channel_while_shimmering(lor_unit_t unit_id, lor_channel_t channel, lor_intensity_t from, lor_intensity_t to, float seconds) {
    struct lor_effect_fade_t effect;
    effect.startIntensity = from;
    effect.endIntensity = to;
    effect.duration = lor_seconds_to_time(seconds);
    
    lor_uint8_t msg[128]; // allocate a small data buffer
    
    int written = 0;
    
    // manual assembly of a complex effect command using the various library methods
    written += lor_write_unit(unit_id, &msg[written]);
    msg[written++] = LOR_EFFECT_SHIMMER; // primary effect, stateless
    written += lor_write_channel(channel, &msg[written]);
    msg[written++] = LOR_EFFECT_FADE; // secondary effect, followed by metadata struct
    written += lor_write_effect_struct(LOR_EFFECT_FADE, &effect, &msg[written]);
    
    // using pre-made helper function from `easy.h`
    written += lor_write_complex_effect(LOR_EFFECT_SHIMMER, LOR_EFFECT_FADE, &effect, channel, unit, &msg[written]);
    
    // ...write msg buffer to destination...
}
```

## Library Structure

### Memory Allocations

liblightorama does not allocate any memory internally, and requires a `lor_uint8_t *` be passed to functions for writing
to
memory. To help size your buffers, consider that while not absolute rule and potentially volatile, even the largest
write call will not write more than 32 bytes.

### Error Values

This library has limited input validation features by design (e.g. liblightorama assumes that buffers are correctly
sized and void pointers are of the correct type). liblightorama consists primarily of encoding/decoding functions which
are mostly used as small elements within function chains when assembling packet structures. Error checking within this
structure (without an error sum type) is cumbersome, repetitive, and likely to be skipped by most programmers.

As encoding/decoding functions do not return error values, you may consider validating the written byte count return
value sum against a manually calculated (or previously valid) sample value.

### Brightness Curves

Brightness curves are responsible for converting a normalized brightness value [0, 1] to
a [LOR protocol brightness value](https://github.com/Cryptkeeper/lightorama-protocol#brightness). The purpose of
different brightness curves is to easily modify the visual appearance of brightness changes without redefining the
underlying normalized values.

#### Pre-implemented Brightness Curves

| Function                      | Description                                                                                           |
|-------------------------------|-------------------------------------------------------------------------------------------------------|
| `lor_intensity_curve_linear`  | Encodes the normalized value as its direct LOR protocol equivalent value without changes              |
| `lor_intensity_curve_squared` | A smoothed version of `lor_intensity_curve_linear` designed to emphasize changes in brightness values |
| `lor_intensity_curve_vendor`  | The most vanilla intensity curve for anyone with the goal of recreating a Light-O-Rama look           |

#### Custom Brightness Curves

Any brightness curve may be implemented assuming it adheres to the `lor_intensity_curve_t` function signature:
`lor_intensity_t (*lor_intensity_curve_t)(float normal)`

Each brightness curve is designed to return a `lor_intensity_t` object representing the LOR protocol equivalent value of
the normalized input, as adapted by the curve. Several consts have been defined
within [`intensity.h`](include/intensity.h) to avoid using magic numbers in your implementations.
See [`intensity.c`](src/intensity.c) for implementation examples.

#### Complex Effects

Complex effects allow the unit to apply two effects simultaneously to a single channel (with limited options). The "
primary" effect, sent first, must be stateless — i.e. no `effectStruct` option — which reduces your options
to `LOR_EFFECT_TWINKLE` and `LOR_EFFECT_SHIMMER`. The "secondary" effect is followed by its respective metadata struct,
restricting it to supporting `LOR_EFFECT_SET_LIGHTS`, `LOR_EFFECT_FADE`, or `LOR_EFFECT_PULSE`.

The underlying LOR protocol functionality seems to only support single channel IDs (i.e. `lor_channel_t`). However, any
single channel ID that is encoded within a single byte, is padded by an additional `0x81` byte. As a result, the encoded
channel routing bytes should always be exactly two bytes in length.

liblightorama has provided a modified `lor_write_channel` implementation (`lor_write_channel2`)
within [`src/easy.c`](src/easy.c) that corresponds to this unique behavior. A pre-made helper function for encoding
complex effects (`lor_write_complex_effect`) is provided as a part of [`include/easy.h`](include/easy.h).

## Compatibility

liblightorama is implemented following [this documentation](https://github.com/Cryptkeeper/lightorama-protocol), which
was built through reverse engineering efforts. As such, liblightorama has limited compatibility testing and may not work
with your hardware. The current library functionality offerings have been tested with the `LOR1602Wg3` & `CTB16PCg3`
hardware models.

## License

See [LICENSE](LICENSE).
