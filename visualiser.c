#include <stdlib.h>
#include <stdio.h>
#include <lua.h>
#include <lualib.h>
#include <GLFW/glfw3.h>

#ifndef NO_POSIX
#include <unistd.h>
#endif

#include "audio.h"
#include "config.h"
#include "graphics.h"
#include "lua_config.h"
#include "sound_io.h"
#include "debug.h"

#ifndef NO_POSIX
int spawn_audio_producer(struct soundinfo *sinfo) {
  int apipe[2];
  if (pipe(apipe)) return -1;
  init_input(apipe[1], sinfo);
  return apipe[0];
}
#endif

int main() {
  struct soundinfo sinfo;
#ifdef NO_POSIX
  puts("Without POSIX, vmv cannot spawn the audio producer itself.");
  puts("Please ensure that vmv receieves 16-bit little endian 44.1kHz raw audio on stdin.");
  FILE *audio_source = stdin;
#else
  int audio_fd = spawn_audio_producer(&sinfo);
  if (audio_fd < 1) {
    perror("Error spawning audio producer");
    return 1;
  }
  FILE *audio_source = fdopen(audio_fd, "rb");
#endif

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW!\n");
    return -1;
  }

  // Defaults
  struct config config = {
    .pos = {0, 0, 400, 600, 0},
    .sinfo = sinfo,
  };
  init_lua("config.lua", &config);

  if (audio_init(audio_source)) {
    perror("Error initialising audio");
    return 1;
  }

  mainloop(&config);

  destroy_input(&sinfo);
}
