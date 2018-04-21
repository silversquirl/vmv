#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <soundio/soundio.h>
#include "audio.h"
#include "graphics.h"
#include "timer.h"
#include "lua_api.h"
#include "debug.h"

GLFWwindow *window;

void resizeCallback(GLFWwindow *win, int w, int h) {
  glViewport(0, 0, w, h);
}

/* Gets information about a specific monitor. Use -1 for the default monitor. */
int get_monitor_details(int id, struct monitor_info *info) {
  GLFWmonitor *monitor;
  if (id < 0) {
    monitor = glfwGetPrimaryMonitor();
  } else {
    int count;
    GLFWmonitor **monitors = glfwGetMonitors(&count);
    if (id > count) {
      fprintf(stderr, "Invalid monitor specified!\n");
      return -1;
    }
    monitor = monitors[id];
  }

  const GLFWvidmode *mode = glfwGetVideoMode(monitor);

  info->width = mode->width;
  info->height = mode->height;
  info->refresh_rate = mode->refreshRate;

  return 0;
}

int mainloop(struct config *config) {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

  if (config->pos.ontop)
    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

#ifdef GLFW_TRANSPARENT_FRAMEBUFFER
  glfwWindowHint(GLFW_DEPTH_BITS, 16);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#endif

  window = glfwCreateWindow(config->pos.width, config->pos.height, "vmv", NULL, NULL);
  glfwSetFramebufferSizeCallback(window, resizeCallback);

  GLFWmonitor *mon;

  if (config->monitor < 0) {
    mon = glfwGetPrimaryMonitor();
  } else {
    int count;
    GLFWmonitor **monitors = glfwGetMonitors(&count);
    if (config->monitor >= count) {
      fprintf(stderr, "Invalid monitor specified!\n");
      return -1;
    }
    mon = monitors[config->monitor];
  }

  int offsetx, offsety; // x and y offsets for the given monitor
  glfwGetMonitorPos(mon, &offsetx, &offsety);
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

  if (config->pos.center) {
    config->pos.x = (mode->width - config->pos.width) / 2;
    config->pos.y = (mode->height - config->pos.height) / 2;
  }
  glfwSetWindowPos(window, offsetx + config->pos.x, offsety + config->pos.y);

  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window!\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  timer_lib_initialize();
  tick_t start = 0, fps_timer = timer_current();

  int frames = 0;

  while (!glfwWindowShouldClose(window)) {
    if (timer_elapsed(fps_timer) >= 1.0f) {
      DEBUG("FPS: %d", frames);
      frames = 0;
      fps_timer = timer_current();
    }

    if (timer_elapsed(start) < 1.0f / config->fps_cap)
      continue;

    process_audio(timer_elapsed(start));

    start = timer_current();

    glClear(GL_COLOR_BUFFER_BIT);

    run_visualisation(config, bars);

    glfwSwapBuffers(window);
    glfwPollEvents();

    if (config->close_key) {
      if (glfwGetKey(window, config->close_key) == GLFW_PRESS) break;
    }
    frames++;
  }

  glfwTerminate();
  timer_lib_shutdown();

  return 0;
}
