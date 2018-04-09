#ifndef __VMV_GRAPHICS_H__
#define __VMV_GRAPHICS_H__

#include <lua.h>
#include <lualib.h>

typedef struct {
  char close_key;
  int fps_cap;
  int monitor; // -1 for default
  struct {
    unsigned int x, y, width, height;
    int center; // When center != 0, x and y are ignored
  } pos;
  lua_State *L;
} graphics_options;

typedef struct {
  unsigned int width, height, refresh_rate;
} monitor_info;

int mainloop(graphics_options p);
int get_monitor_details(int id, monitor_info *info); // id is -1 for default monitor

#endif
