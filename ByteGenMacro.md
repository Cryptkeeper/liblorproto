# Byte Generation Macro

I recently tested a macro for generating inline byte data. Ideally, this would allow simplified use of the encoding
functions, replacing a series of function calls with compile-time inline byte data for pre-defined effects.

For example, this function would set channel 10 (index 9) on unit 1 to 50% brightness.

```c
// existing encoding functions
void turn_on_channel_ten(void) {
    uint8_t b[32];
    struct lor_effect_setintensity_t effectStruct = {
            .intensity = lor_intensity_curve_linear(0.5),
    };
    int n = lor_write_channel_effect(LOR_EFFECT_SET_INTENSITY, &effectStruct, (lor_channel_t) 9, (lor_unit_t) 1, &b[0]);
    // n == 6
}
```

```c
// using inline bytegen macro
void turn_on_channel_ten(void) {
    unsigned char b[] = {lor_set(1, 9, 0.5)}; // expands to {0x00, 0x01, 0x03, 0x78, 0x89, 0x00,}
    // sizeof(b) == 6
}
```

I opted not to include the macro in the library due to its lack of encoding compression opportunities and limited
flexibility (anything with a conditional encoding length is going to be a headache).

However, it is a handy tool for simple projects and debugging purposes. I've removed any liblightorama dependent code,
and instead made it available as a standalone macro. Pairing this macro with a serial port library and a heartbeat timer
provides everything you need to control Light-O-Rama hardware with your own software.

```c
// params: 
//  unsigned char unit, unsigned char channel, float intensity
#define lor_set(unit, channel, intensity)                   \
  0x00,                                                     \
  ((unsigned char)unit),                                    \
  0x03,                                                     \
  ((((float)intensity) * (0x01 - 0xF0)) + 0xF0),            \
  (0x80 | ((unsigned char)channel)),                        \
  0x00,
```

**Additional Notes**

* `lor_set` uses a simplified version of the `lor_intensity_curve_linear` intensity function
* The `channel` value must be <64, as the inline encoding does not support offsets like [`uid.c`](src/uid.c)
* The intensity encoding portion has been left somewhat expanded for easier customization
