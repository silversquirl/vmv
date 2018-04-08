#include <stdlib.h>
#include <stdio.h>
#include "audio.h"

int main() {
  audio_init();
  for (;;) {
    process_audio();
    for (size_t i = 0; i < bars.len; ++i)
      printf("%u ", bars.buf[i]);
    putchar('\n');
  }
}
