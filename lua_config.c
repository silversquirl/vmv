#include <stdio.h>
#include <string.h>
#include <lauxlib.h>
#include "lua_api.h"
#include "lua_config.h"
#include "debug.h"

int init_lua(const char *config_file, struct config *config) {
  config->lua = luaL_newstate();
  if (!config->lua)
    // TODO: add error codes
    return -1;

  luaL_openlibs(config->lua);

  if (luaL_loadfile(config->lua, config_file))
    // TODO: add error codes
    return -1;

  init_api(config);

  if (lua_pcall(config->lua, 0, 1, 0))
    // TODO: add error codes
    return -1;

  DEBUG("Ran config");

  if (lua_getfield(config->lua, -1, "visualiser") != LUA_TFUNCTION)
    // TODO: add error codes
    return -1;
  DEBUG("Found visualiser");
  set_visualiser(config);
  DEBUG("Set visualiser");

  lua_pop(config->lua, 1); // Pop config table

  DEBUG("Done!");

  return 0;
}

