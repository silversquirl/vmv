#include <lauxlib.h>
#include <string.h>

#include <stdio.h>

#include "lua_api.h"

lua_State *L;

int init_lua(const char *config_file, lua_State **setL) {
  L = luaL_newstate();

  if (!L)
    return -1;

  luaL_openlibs(L);

  if (luaL_loadfile(L, config_file))
    return -1;

  init_api(L);

  if (lua_pcall(L, 0, 1, 0))
    return -1;

  printf("Ran config\n");

  /* Config table on stack */

  lua_pushnil(L);

  while (lua_next(L, -2)) {
    printf("Read config value\n");
    if (lua_type(L, -2) == LUA_TSTRING) {
      printf("Key is string\n");
      const char *k = lua_tolstring(L, -2, NULL);
      if (!strcmp(k, "visualiser")) {
        printf("Set visualiser\n");
        visualiser_ref = luaL_ref(L, LUA_REGISTRYINDEX);
        continue; // luaL_ref() pops the value off the stack for us
      }
    }
    lua_pop(L, 1);
  }

  printf("Done!\n");

  *setL = L;
  return 0;
}

