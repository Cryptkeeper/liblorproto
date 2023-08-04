# liblightorama

An unofficial C99 library implementing the [Light-O-Rama (LOR) communication protocol](https://github.com/Cryptkeeper/lightorama-protocol) alongside related APIs, aiming to reduce usage of magic values and competing encoding & helper methods.

Usage of liblightorama assumes existing familiarity with the [LOR protocol](https://github.com/Cryptkeeper/lightorama-protocol).

## Project Structure

| Header                                          | Purpose                                                                                                                                                                             |
|-------------------------------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| [`lightorama/coretypes.h`](include/lightorama/coretypes.h) | Core typedef definitions and fixed size ints used by the library, likely auto included by another header                                                                            |
| [`lightorama/easy.h`](include/lightorama/easy.h)           | Ready to go functions for encoding predefined effect structures to a given unit/channel/channel set                                                                                 |
| [`lightorama/effect.h`](include/lightorama/effect.h)       | Type definitions for effect related data structures and associated encoding functions                                                                                               |
| [`lightorama/heartbeat.h`](include/lightorama/heartbeat.h) | Heartbeat encoding functions and associated named helper constants                                                                                                                  |
| [`lightorama/intensity.h`](include/lightorama/intensity.h) | Type definitions for intensity, named helper constants, and brightness curve functions for encoding/decoding normalized values                                                      |
| [`lightorama/model.h`](include/lightorama/model.h)         | Automatically generated code representation of [LOR_DeviceFile.txt](http://www1.lightorama.com/downloads/LOR_DeviceFile.txt), includes helper functions for referencing model names |
| [`lightorama/time.h`](include/lightorama/time.h)           | Type definitions for time durations, named helper constants, and functions for encoding/decoding normalized values                                                                  |
| [`lightorama/uid.h`](include/lightorama/uid.h)             | Type definitions, named helper constants, and encoding/decoding functions for any referenceable addresses (unit IDs, channel IDs)                                                   |
| [`lightorama/version.h`](include/lightorama/version.h)     | Named constants of liblightorama version information                                                                                                                                |

You may optionally use `lightorama/lightorama.h` to automatically include all library header files (except `model.h` due to its strings, please include as needed).

liblightorama strives to be fairly portable (typically for use in microcontrollers) and requires no external dependencies or headers. The few core data types used by liblightorama (e.g. fixed size ints and booleans) are defined in [`coretypes.h`](include/lightorama/coretypes.h). Limited use of floating-point arithmetic is present in the [`intensity.c`](src/intensity.c) math functions.

## Installation

liblightorama uses [CMake](https://cmake.org/) for building and packaging the library.

1. Generate Makefiles using `cmake .`
2. (Optional) If outdated, use `python3 tools/generate_model_files.py` to generate updated `model.h` & `model.c` code files
3. Compile the library using `make`
4. Optionally install the headers and compiled archive using `make install`
5. Include your selected headers, or include all headers using `lightorama/lightorama.h`

If optionally installed, `install_manifest.txt` will be created, containing the installed file paths for easy removal.

## Usage Examples

See [`examples/common-methods.c`](examples/common-methods.c) for usage examples.

## Packet Structure

The majority of data is lighting control packets. Each follows a basic structure, prefixed by the target unit ID, and followed by a table directory and its corresponding data entry rows, in matching order. The corresponding data structures used by liblightorama for each field are shown in the diagram below.

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

* Additional packet structures exist for remote management/firmware update/system configuration feature sets, but are entirely undocumented and more time-consuming to explore. As a result, liblightorama focuses purely on lighting control packets.
*  A `LorAnyArgs` type has been provided to reduce code complexity when passing effect arguments. It is solely a union of the other individually defined effect argument structures.

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

Within liblightorama a single channel ID is represented by the `LorChannel` type. Keep in mind that channels within the LOR protocol start at 0, not 1. A `LorChannelSet` can be used to reference a single channel, but that is likely a waste of bandwidth and isn't recommended.

For using channel groups and channel banks, the `LorChannelSet` type contains a `channels` field for defining the channel banks and an optional `offset` value for providing a channel group offset. liblightorama's `LorChannelSet` encoding logic will apply a few protocol optimizations that can help minimize memory usage when referencing a single channel bank.

Depending on your type (`LorChannel`, `LorChannelSet`, `LorUnit`) you will use different functions when encoding/decoding (e.g. `lorEncodeChannelEffect` vs `lorEncodeChannelSetEffect` vs `lorEncodeUnitEffect`). A missing function signature for your type may mean it is unsupported by the protocol.

### Channel Set Code Examples

```C
LorChannelSet first_8_channels = { 
        .channels = 0xFF00, // set first 8 high bits
        .offset = 0, // channel group 0
};

LorChannelSet channels_96_to_112 = {
        .channels = 0xFFFF, // all 16 channels
        .offset = 5, // start at channel 96 (96/16 == 6 - 1 [because group #1 is 0] == 5)
};

LorChannelSet only_channel_32 = {
        .channels = 0x1, // only set lowest order bit (channel 16)
        .offset = 1, // offset channels by +16
};
```

## Library Structure

### Memory Allocations

liblightorama does not allocate any memory internally, and requires a `void write(unsigned char b)` function be passed to functions for writing to memory. To help size your buffers, consider that while not absolute rule and potentially volatile, even the largest write call will not write more than 32 bytes.

### Error Values

Due to the simplicity of most encoding routines, many functions do not return a value. Those which provide basic value checking (where possible) return type `bool` with true indicating an encoding success. No further error information is available and users should check the function implementation for failure causes. I recommend a debug mode in your application which prints the basic routing information (unit, channel, effect args) prior to invoking encoding calls.

### Brightness Curves

Brightness curves are responsible for converting a normalized brightness value [0, 1] to a [LOR protocol brightness value](https://github.com/Cryptkeeper/lightorama-protocol#brightness). The purpose of different brightness curves is to easily modify the visual appearance of brightness changes without redefining the underlying normalized values.

#### Pre-implemented Brightness Curves

| Function                   | Description                                                                                           |
|----------------------------|-------------------------------------------------------------------------------------------------------|
| `lorIntensityCurveLinear`  | Encodes the normalized value as its direct LOR protocol equivalent value without changes              |
| `lorIntensityCurveSquared` | A smoothed version of `lorIntensityCurveLinear` designed to emphasize changes in brightness values    |
| `lorIntensityCurveVendor`  | The most vanilla intensity curve for anyone with the goal of recreating a Light-O-Rama look           |

#### Custom Brightness Curves

Any brightness curve may be implemented assuming it adheres to the `LorIntensityCurveFn` function signature: `LorIntensity (*LorIntensityCurveFn)(float normal)`

Each brightness curve is designed to return a `LorIntensity` value representing the LOR protocol equivalent value of the normalized input, as adapted by the curve. Several consts have been defined within [`intensity.h`](include/lightorama/intensity.h) to avoid using magic numbers in your implementations. See [`intensity.c`](src/intensity.c) for implementation examples.

#### Layered Effects

Layered effects allow the unit to apply two effects simultaneously to a single channel (with limited options). The "primary" effect, sent first, must be stateless — i.e. no `args` option — which reduces your options to `LOR_EFFECT_TWINKLE` and `LOR_EFFECT_SHIMMER`. The "secondary" effect is followed by its respective metadata struct, restricting it to supporting `LOR_EFFECT_SET_LIGHTS`, `LOR_EFFECT_FADE`, or `LOR_EFFECT_PULSE`.

The underlying LOR protocol functionality seems to only support single channel IDs (i.e. `LorChannel`). However, any single channel ID that is encoded within a single byte, is padded by an additional `0x81` byte. As a result, the encoded channel routing bytes should always be exactly two bytes in length.

liblightorama has provided a `lorEncodeChannel2` function within [`uid.h`](include/lightorama/uid.h) that when invoked with the `align` parameter set to 16 (normally 8), will result in a `0x81` padded 16-byte channel encoding. A pre-made helper function for encoding layered effects (`lorEncodeLayeredChannelEffect`) is provided as a part of [`easy.h`](include/lightorama/easy.h).

## Compatibility

liblightorama is implemented following [this documentation](https://github.com/Cryptkeeper/lightorama-protocol), which was built through reverse engineering efforts. As such, liblightorama has limited compatibility testing and may not work with your hardware. The current library functionality offerings have been tested with the `LOR1602Wg3` & `CTB16PCg3` hardware models.

## License

See [LICENSE](LICENSE).
