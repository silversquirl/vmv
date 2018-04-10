#include <stdlib.h>
#include <stdio.h>

#ifndef NO_POSIX
#include <unistd.h>
#endif

#include "audio.h"
#include "config.h"
#include "graphics.h"

#ifndef NO_POSIX
int spawn_audio_producer(void) {
  int apipe[2];
  if (pipe(apipe)) return -1;
  switch (fork()) {
  case -1:
    return -1;
  case 0:
    dup2(apipe[1], STDOUT_FILENO);
    execvp(audio_producer[0], audio_producer);
    return -1;
  }
  return apipe[0];
}
#endif

int main() {
#ifdef NO_POSIX
  puts("Without POSIX, vmv cannot spawn the audio producer itself.");
  puts("Please ensure that vmv receieves 16-bit little endian 44.1kHz raw audio on stdin.");
  FILE *audio_source = stdin;
#else
  int audio_fd = spawn_audio_producer();
  if (audio_fd < 1) {
    perror("Error spawning audio producer");
    return 1;
  }
  FILE *audio_source = fdopen(audio_fd, "rb");
#endif

  if (audio_init(audio_source)) {
    perror("Error initialising audio");
    return 1;
  }

  graphics_options gopt = {
    .fps_cap = 60,
    .bar_color = { 1.0, 0.0, 1.0 },
    .spacing = 0.1f,
    .vertical_scale = 0.95f,
    .close_key = 0,
    .monitor = -1,
    .pos = {0, 0, 400, 600, 0},
  };

  mainloop(gopt);
}
