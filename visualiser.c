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

int main() {
  struct soundinfo sinfo;
  init_input(&sinfo);

  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW!\n");
    return -1;
  }

  // Defaults
  struct config config = {
    .pos = {0, 0, 400, 600, 0},
  };

  init_lua("config.lua", &config);
  config.sinfo = sinfo;

  if (audio_init(&config)) {
    perror("Error initialising audio");
    return 1;
  }

  mainloop(&config);

  destroy_input(&sinfo);
}
