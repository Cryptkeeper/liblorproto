#include "../include/time.h"

#define LOR_CLAMPF(x, min, max) (x < min ? min : (x > max ? max : x))

lor_time_t lor_seconds_to_time(float seconds) {
  seconds = LOR_CLAMPF(seconds, LOR_TIME_MIN_SECONDS, LOR_TIME_MAX_SECONDS);
  return (lor_time_t)(LOR_TIME_MIN / (seconds / LOR_TIME_MIN_SECONDS));
}

float lor_time_to_seconds(uint16_t time) {
  const float seconds = (LOR_TIME_MIN / (float)time) * LOR_TIME_MIN_SECONDS;
  return LOR_CLAMPF(seconds, LOR_TIME_MIN_SECONDS, LOR_TIME_MAX_SECONDS);
}

#define LOR_DURATION_MASK 0x8000

int lor_write_time(lor_time_t time, uint8_t *b) {
  // include required 0x8000 protocol mask if the upper duration byte is zero
  b[0] = ((time | LOR_DURATION_MASK) & 0xFF00) >> 8;
  b[1] = (time & 0x00FF);
  return 2;
}

int lor_read_time(lor_time_t *time, const uint8_t *b) {
  // strip protocol mask bit (indicates the byte value is zero)
  const uint16_t t = ((b[0] << 8) & b[1]) & ~LOR_DURATION_MASK;
  *time = (lor_time_t)(t);
  return 2;
}
