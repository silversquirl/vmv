#include <stdio.h>
#include <string.h>
#include <lauxlib.h>
#include "lua_api.h"
#include "lua_config.h"
#include "debug.h"
#include "graphics.h"
#include <strings.h>

#include <GLFW/glfw3.h>

struct key_pair {const char *name; int key;};
const struct key_pair keys[] = {
  {"a", GLFW_KEY_A},
  {"b", GLFW_KEY_B},
  {"c", GLFW_KEY_C},
  {"d", GLFW_KEY_D},
  {"e", GLFW_KEY_E},
  {"f", GLFW_KEY_F},
  {"g", GLFW_KEY_G},
  {"h", GLFW_KEY_H},
  {"i", GLFW_KEY_I},
  {"j", GLFW_KEY_J},
  {"k", GLFW_KEY_K},
  {"l", GLFW_KEY_L},
  {"m", GLFW_KEY_M},
  {"n", GLFW_KEY_N},
  {"o", GLFW_KEY_O},
  {"p", GLFW_KEY_P},
  {"q", GLFW_KEY_Q},
  {"r", GLFW_KEY_R},
  {"s", GLFW_KEY_S},
  {"t", GLFW_KEY_T},
  {"u", GLFW_KEY_U},
  {"v", GLFW_KEY_V},
  {"w", GLFW_KEY_W},
  {"x", GLFW_KEY_X},
  {"y", GLFW_KEY_Y},
  {"z", GLFW_KEY_Z},
  {"0", GLFW_KEY_0},
  {"1", GLFW_KEY_1},
  {"2", GLFW_KEY_2},
  {"3", GLFW_KEY_3},
  {"4", GLFW_KEY_4},
  {"5", GLFW_KEY_5},
  {"6", GLFW_KEY_6},
  {"7", GLFW_KEY_7},
  {"8", GLFW_KEY_8},
  {"9", GLFW_KEY_9},
  {NULL},
};

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

  if (lua_getfield(config->lua, -1, "monitor") == LUA_TNUMBER) {
    DEBUG("Setting to monitor %lld", lua_tointeger(config->lua, -1));
    config->monitor = lua_tointeger(config->lua, -1);
  } else {
    DEBUG("No monitor specified, assuming default");
    config->monitor = -1;
  }
  lua_pop(config->lua, 1);

  if (lua_getfield(config->lua, -1, "fps_cap") == LUA_TNUMBER) {
    DEBUG("Setting FPS cap to %lld", lua_tointeger(config->lua, -1));
    config->fps_cap = lua_tointeger(config->lua, -1);
  } else {
    DEBUG("FPS cap not set, finding monitor refresh rate...");
    struct monitor_info info;
    if (!get_monitor_details(config->monitor, &info)) {
      DEBUG("Using %d as FPS cap", info.refresh_rate);
      config->fps_cap = info.refresh_rate;
    } else {
      DEBUG("Could not find refresh rate, using 60 as FPS cap");
      config->fps_cap = 60;
    }
  }
  lua_pop(config->lua, 1);

  config->close_key = 0;
  if (lua_getfield(config->lua, -1, "close_key") == LUA_TSTRING) {
    const char *close_key = lua_tolstring(config->lua, -1, NULL);
    for (const struct key_pair *k = keys; k->name; ++k) {
      if (!strcasecmp(close_key, k->name)) {
        DEBUG("Found valid close_key value");
        config->close_key = k->key;
        break;
      }
    }
    if (config->close_key == 0)
      DEBUG("Invalid close_key value");
  } else {
    DEBUG("No close_key specified, assuming none");
  }
  lua_pop(config->lua, 1);

  if (lua_getfield(config->lua, -1, "x") == LUA_TNUMBER) {
    DEBUG("Setting x to %lld", lua_tointeger(config->lua, -1));
    config->pos.x = lua_tointeger(config->lua, -1);
  }
  lua_pop(config->lua, 1);

  if (lua_getfield(config->lua, -1, "y") == LUA_TNUMBER) {
    DEBUG("Setting y to %lld", lua_tointeger(config->lua, -1));
    config->pos.y = lua_tointeger(config->lua, -1);
  }
  lua_pop(config->lua, 1);

  if (lua_getfield(config->lua, -1, "width") == LUA_TNUMBER) {
    DEBUG("Setting width to %lld", lua_tointeger(config->lua, -1));
    config->pos.width = lua_tointeger(config->lua, -1);
  }
  lua_pop(config->lua, 1);

  if (lua_getfield(config->lua, -1, "height") == LUA_TNUMBER) {
    DEBUG("Setting height to %lld", lua_tointeger(config->lua, -1));
    config->pos.height = lua_tointeger(config->lua, -1);
  }
  lua_pop(config->lua, 1);

  config->pos.center = 0;
  if (lua_getfield(config->lua, -1, "center") == LUA_TBOOLEAN) {
    DEBUG("Setting center to %d", lua_toboolean(config->lua, -1));
    config->pos.center = (char)lua_toboolean(config->lua, -1);
    if (config->pos.center)
      DEBUG("Ignoring x, y");
  }
  lua_pop(config->lua, 1);

  config->pos.ontop = 0;
  if (lua_getfield(config->lua, -1, "always_on_top") == LUA_TBOOLEAN) {
    DEBUG("Setting always_on_top to %d", lua_toboolean(config->lua, -1));
    config->pos.ontop = (char)lua_toboolean(config->lua, -1);
  }
  lua_pop(config->lua, 1);

  lua_pop(config->lua, 1); // Pop config table

  DEBUG("Done!");

  return 0;
}

