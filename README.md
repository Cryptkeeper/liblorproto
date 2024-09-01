# liblorproto

An **unofficial** C99 library implementing the [Light-O-Rama (LOR) communication protocol](https://github.com/Cryptkeeper/lightorama-protocol) alongside related APIs, aiming to reduce usage of magic values and competing encoding & helper methods. Previously called liblightorama.

Usage of liblorproto assumes existing familiarity with the [LOR protocol](https://github.com/Cryptkeeper/lightorama-protocol).

The previous v2.x release is available by checking out the `v2.x` branch. Only v3.x is actively supported by the author.

**Archived in favor of my work on [libtinylor](https://github.com/Cryptkeeper/libtinylor)**

## Project Structure

| Header                                                 | Purpose                                                                                                                                                                           |
| ------------------------------------------------------ | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [`lorproto/coretypes.h`](include/lorproto/coretypes.h) | Core typedef definitions and fixed size ints used by the library, likely auto included by another header                                                                          |
| [`lorproto/compress.h`](include/lorproto/compress.h)   | LOR-protocol specific multi-channel update compression utility                                                                                                                    |
| [`lorproto/easy.h`](include/lorproto/easy.h)           | Ready to go functions for encoding predefined effect structures to a given unit/channel/channel set                                                                               |
| [`lorproto/effect.h`](include/lorproto/effect.h)       | Type definitions for effect related data structures and associated encoding functions                                                                                             |
| [`lorproto/heartbeat.h`](include/lorproto/heartbeat.h) | Heartbeat encoding functions and associated named helper constants                                                                                                                |
| [`lorproto/intensity.h`](include/lorproto/intensity.h) | Type definitions for intensity, named helper constants, and brightness curve functions for encoding/decoding normalized values                                                    |
| [`lorproto/model.h`](include/lorproto/model.h)         | Automatically generated code representation of [LOR_DeviceFile.txt](http://www1.lorproto.com/downloads/LOR_DeviceFile.txt), includes helper functions for referencing model names |
| [`lorproto/time.h`](include/lorproto/time.h)           | Type definitions for time durations, named helper constants, and functions for encoding/decoding normalized values                                                                |
| [`lorproto/uid.h`](include/lorproto/uid.h)             | Type definitions, named helper constants, and encoding/decoding functions for any referenceable addresses (unit IDs, channel IDs)                                                 |
| [`lorproto/version.h`](include/lorproto/version.h)     | Named constants of liblorproto version information                                                                                                                                |

You may optionally use `lorproto/lorproto.h` to automatically include all library header files (except `model.h` due to its strings, please include as needed).

liblorproto strives to be fairly portable (typically for use in microcontrollers) and requires no external dependencies or headers. The few core data types used by liblorproto (e.g. fixed size ints and booleans) are defined in [`coretypes.h`](include/lorproto/coretypes.h). Limited use of floating-point arithmetic is present in the [`intensity.c`](src/intensity.c) math functions.

## Installation

liblorproto uses [CMake](https://cmake.org/) for building and packaging the library.

1. Generate Makefiles using `cmake .`
2. (Optional) If outdated, use `python3 tools/generate_model_files.py` to generate updated `model.h` & `model.c` code files
3. Compile the library using `make`
4. Optionally install the headers and compiled archive using `make install`
5. Include your selected headers, or include all headers using `lorproto/lorproto.h`

If optionally installed, `install_manifest.txt` will be created, containing the installed file paths for easy removal.

## Usage Examples

See [`examples/common-methods.c`](examples/common-methods.c) for usage examples.

## Packet Structure

The majority of data is lighting control packets. Each follows a basic structure, prefixed by the target unit ID, and followed by a table directory and its corresponding data entry rows, in matching order. The corresponding data structures used by liblorproto for each field are shown in the diagram below.

```
Light Control Packet
┌─────────────────┐
│Unit             ├─► LorUnit
├────────┬────────┤
│Effect  │Channel ├─► LorChannelFormat (4 bits)
├────────┴────────┤   LorChannel (4 bits)
│Variable length  │
│effect struct    ├─► Lor*Args
│                 │
│[0,4] bytes      │
├─────────────────┤
│Variable length  │
│channel struct   ├─► LorChannelSet
│                 │   LorChannel
│[1,2] bytes      │
└─────────────────┘
```

### Implementation Notes

- Additional packet structures exist for remote management/firmware update/system configuration feature sets, but are entirely undocumented and more time-consuming to explore. As a result, liblorproto focuses purely on lighting control packets.
- A `union LorEffectArgs` type has been provided to reduce code complexity when passing effect arguments. It is solely a union of the other individually defined effect argument structures.

## Referencing Channels

The LOR protocol has four distinct ways to reference channels. Each becomes increasingly complex, but at the benefit of far more channel control options—dramatically reducing bandwidth use compared to individual channels.

1. Using a single channel ID.
2. Using 1-2 "channel bank" bit masks for selecting up to 16 sequential channels. A single channel bank is an 8 bit mask. Each "channel group" (16 channels) therefore contains two channel banks, named "high" (channels 1-8, first channel bank) and "low" (channels 9-16, second channel bank). These seemingly backwards names come from their bit order when encoded into the LOR protocol, with the "high" channel bank becoming the highest order bits and vice versa.
3. Using the previous mode with a channel group offset, which offsets the channel bank masks by a multiple of 16 (the size of a channel group).
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

Within liblorproto a single channel ID is represented by the `LorChannel` type. Keep in mind that channels within the LOR protocol start at 0, not 1. A `LorChannelSet` can be used to reference a single channel, but that is likely a waste of bandwidth and isn't recommended.

For using channel groups and channel banks, the `LorChannelSet` type contains a `channels` field for defining the channel banks and an optional `offset` value for providing a channel group offset. liblorproto's `LorChannelSet` encoding logic will apply a few protocol optimizations that can help minimize memory usage when referencing a single channel bank.

Depending on your type (`LorChannel`, `LorChannelSet`, `LorUnit`) you will use different functions when encoding/decoding (e.g. `lorAppendChannelEffect` vs `lorAppendChannelSetEffect` vs `lorAppendUnitEffect`). A missing function signature for your type may mean it is unsupported by the protocol.

### Channel Set Code Examples

```C
LorChannelSet first_8_channels = {
        .channelBits = 0xFF00, // set first 8 high bits
        .offset = 0, // channel group 0
};

LorChannelSet channels_96_to_112 = {
        .channelBits = 0xFFFF, // all 16 channels
        .offset = 5, // start at channel 96 (96/16 == 6 - 1 [because group #1 is 0] == 5)
};

LorChannelSet only_channel_32 = {
        .channelBits = 0x1, // only set lowest order bit (channel 16)
        .offset = 1, // offset channels by +16
};
```

## Library Structure

### Memory Allocations

liblorproto does not allocate any memory internally, and requires a `LorBuffer *` struct be passed to functions for writing to memory (with basic write bounds checking via an `assert` call). To help size your buffers, consider that while not absolute rule and potentially volatile, even the largest single lighting control packet will not write more than 32 bytes.

### Error Values

Due to the simplicity of most encoding routines encoding functions do not return a value. A select few encoding issues (e.g. overflow/underflow issues) may be raised via `assert` which I recommend enabling in development builds. In most cases, there is no strong ability for the library to identify invalid values given the protocol's unknown limitations besides my adhoc testing.

### Brightness Curves

Brightness curves are responsible for converting a normalized brightness value [0, 1] to a [LOR protocol brightness value](https://github.com/Cryptkeeper/lorproto-protocol#brightness). The purpose of different brightness curves is to easily modify the visual appearance of brightness changes without redefining the underlying normalized values.

#### Pre-implemented Brightness Curves

| Function                   | Description                                                                                        |
| -------------------------- | -------------------------------------------------------------------------------------------------- |
| `LorIntensityCurveLinear`  | Encodes the normalized value as its direct LOR protocol equivalent value without changes           |
| `LorIntensityCurveSquared` | A smoothed version of `lorIntensityCurveLinear` designed to emphasize changes in brightness values |
| `LorIntensityCurveVendor`  | The most vanilla intensity curve for anyone with the goal of recreating a Light-O-Rama look        |

#### Custom Brightness Curves

Any brightness curve may be implemented assuming it adheres to the `LorIntensityCurveFn` function signature: `LorIntensity (*LorIntensityCurveFn)(float normal)`

Each brightness curve is designed to return a `LorIntensity` value representing the LOR protocol equivalent value of the normalized input, as adapted by the curve. Several consts have been defined within [`intensity.h`](include/lorproto/intensity.h) to avoid using magic numbers in your implementations. See [`intensity.c`](src/intensity.c) for implementation examples.

#### Layered Effects

Layered effects allow the unit to apply two effects simultaneously to a single channel (with limited options). The "primary" effect, sent first, must be stateless — i.e. no `args` option — which reduces your options to `LOR_EFFECT_TWINKLE` and `LOR_EFFECT_SHIMMER`. The "secondary" effect is followed by its respective metadata struct, restricting it to supporting `LOR_EFFECT_SET_INTENSITY`, `LOR_EFFECT_FADE`, or `LOR_EFFECT_PULSE`.

The underlying LOR protocol functionality seems to only support single channel IDs (i.e. `LorChannel`). However, any single channel ID that is encoded within a single byte, is padded by an additional `0x81` byte. As a result, the encoded channel routing bytes should always be exactly two bytes in length.

liblorproto has provided a `lorAppendAlignedChannel` function within [`uid.h`](include/lorproto/uid.h) that when invoked with the `align` parameter set to `LOR_ALIGN_16` (normally `LOR_ALIGN_8`), will result in a `0x81` padded 16-byte channel encoding. A pre-made helper function for encoding layered effects (`lorAppendLayeredChannelEffect`) is provided as a part of [`easy.h`](include/lorproto/easy.h).

## Compatibility

liblorproto is implemented following [this documentation](https://github.com/Cryptkeeper/lorproto-protocol), which was built through reverse engineering efforts. As such, liblorproto has limited compatibility testing and may not work with your hardware. The current library functionality offerings have been tested with the `LOR1602Wg3` & `CTB16PCg3` hardware models.

## License

See [LICENSE](LICENSE).
