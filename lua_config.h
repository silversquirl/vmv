#ifndef __VMV_LUA_CONFIG_H__
#define __VMV_LUA_CONFIG_H__

#include <lua.h>
#include <lualib.h>

struct config {
  lua_State *lua;

  // Graphics
  char close_key;
  int fps_cap;
  int monitor; // -1 for default
  struct {
    unsigned x, y, width, height;
    char center; // When true, x and y are ignored
  } pos;
  int visualiser_ref;
};

int init_lua(const char *config_file, struct config *config);


#endif
