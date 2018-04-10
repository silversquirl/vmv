#ifndef __VMV_LUA_CONFIG_H__
#define __VMV_LUA_CONFIG_H__

#include <lua.h>
#include <lualib.h>

#include "sound_io.h"

struct config {
  lua_State *lua;

  // Graphics
  char close_key;
  int fps_cap;
  int monitor; // -1 for default
  struct {
    unsigned x, y, width, height;
    char center; // When true, x and y are ignored
    char ontop;
  } pos;
  int visualiser_ref;
  struct soundinfo sinfo;
};

int init_lua(const char *config_file, struct config *config);


#endif
