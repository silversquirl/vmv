#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "audio.h"
#include "graphics.h"
#include "timer.h"
#include "lua_api.h"

GLFWwindow *window;

void resizeCallback(GLFWwindow *win, int w, int h) {
  glViewport(0, 0, w, h);
}

/* Gets information about a specific monitor. Use -1 for the default monitor. */
int get_monitor_details(int id, monitor_info *info) {
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

int mainloop(graphics_options p) {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW!\n");
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
#ifdef GLFW_TRANSPARENT_FRAMEBUFFER
  glfwWindowHint(GLFW_DEPTH_BITS, 16);
  glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#endif

  window = glfwCreateWindow(p.pos.width, p.pos.height, "vmv", NULL, NULL);
  glfwSetFramebufferSizeCallback(window, resizeCallback);

  GLFWmonitor *mon;

  if (p.monitor < 0) {
    mon = glfwGetPrimaryMonitor();
  } else {
    int count;
    GLFWmonitor **monitors = glfwGetMonitors(&count);
    if (p.monitor >= count) {
      fprintf(stderr, "Invalid monitor specified!\n");
      return -1;
    }
    mon = monitors[p.monitor];
  }

  int offsetx, offsety; // x and y offsets for the given monitor
  glfwGetMonitorPos(mon, &offsetx, &offsety);
  const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

  if (p.pos.center) {
    glfwSetWindowPos(window, offsetx + mode->width / 2 - p.pos.width / 2, offsety + mode->height / 2 - p.pos.height / 2);
  } else {
    glfwSetWindowPos(window, offsetx + p.pos.x, offsety + p.pos.y);
  }

  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window!\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW!\n");
    glfwTerminate();
    return -1;
  }

  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  timer_lib_initialize();
  tick_t start = 0;

  do {
    process_audio();
    if (timer_elapsed(start) < 1.0f / p.fps_cap)
      continue;

    start = timer_current();

    glClear(GL_COLOR_BUFFER_BIT);

    run_visualisation(p.L, bars.buf, bars.len);

    glfwSwapBuffers(window);
    glfwPollEvents();
  } while ((p.close_key >= 0 ? glfwGetKey(window, p.close_key) != GLFW_PRESS : 1) && !glfwWindowShouldClose(window));

  glfwTerminate();
  timer_lib_shutdown();

  return 0;
}
