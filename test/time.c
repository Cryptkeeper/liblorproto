#include "../include/lightorama.h"

#include <assert.h>
#include <stdio.h>

int main() {
  assert(lor_seconds_to_time(0.1F) == 5099);
  assert(lor_seconds_to_time(0.5F) == 1019);
  assert(lor_seconds_to_time(1.0F) == 509);
  assert(lor_seconds_to_time(2.0F) == 254);
  assert(lor_seconds_to_time(25.0F) == 20);

  float intervals[5] = {0.1F, 0.5F, 1.0F, 2.0F, 25.0F};

  size_t i;
  for (i = 0; i < sizeof(intervals) / sizeof(float); i++) {
    const float input = intervals[i];
    const float output = lor_time_to_seconds(lor_seconds_to_time(input));

    printf("%f %f\n", input, output);

    // safely handle an offset between the expected seconds values
    // this is caused by the encoding scheme being lossy with precision
    const float diff = output > input ? output - input : input - output;

    assert(diff < 0.01);
  }

  return 0;
}
