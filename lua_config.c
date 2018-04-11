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
  {"space", GLFW_KEY_SPACE},
  {"apostrophe", GLFW_KEY_APOSTROPHE},
  {"comma", GLFW_KEY_COMMA},
  {"minus", GLFW_KEY_MINUS},
  {"period", GLFW_KEY_PERIOD},
  {"slash", GLFW_KEY_SLASH},
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
  {"semicolon", GLFW_KEY_SEMICOLON},
  {"equal", GLFW_KEY_EQUAL},
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
  {"left_bracket", GLFW_KEY_LEFT_BRACKET},
  {"backslash", GLFW_KEY_BACKSLASH},
  {"right_bracket", GLFW_KEY_RIGHT_BRACKET},
  {"grave_accent", GLFW_KEY_GRAVE_ACCENT},
  {"world_1", GLFW_KEY_WORLD_1},
  {"world_2", GLFW_KEY_WORLD_2},

  /* Function keys */
  {"escape", GLFW_KEY_ESCAPE},
  {"enter", GLFW_KEY_ENTER},
  {"tab", GLFW_KEY_TAB},
  {"backspace", GLFW_KEY_BACKSPACE},
  {"insert", GLFW_KEY_INSERT},
  {"delete", GLFW_KEY_DELETE},
  {"right", GLFW_KEY_RIGHT},
  {"left", GLFW_KEY_LEFT},
  {"down", GLFW_KEY_DOWN},
  {"up", GLFW_KEY_UP},
  {"page_up", GLFW_KEY_PAGE_UP},
  {"page_down", GLFW_KEY_PAGE_DOWN},
  {"home", GLFW_KEY_HOME},
  {"end", GLFW_KEY_END},
  {"caps_lock", GLFW_KEY_CAPS_LOCK},
  {"scroll_lock", GLFW_KEY_SCROLL_LOCK},
  {"num_lock", GLFW_KEY_NUM_LOCK},
  {"print_screen", GLFW_KEY_PRINT_SCREEN},
  {"pause", GLFW_KEY_PAUSE},
  {"f1", GLFW_KEY_F1},
  {"f2", GLFW_KEY_F2},
  {"f3", GLFW_KEY_F3},
  {"f4", GLFW_KEY_F4},
  {"f5", GLFW_KEY_F5},
  {"f6", GLFW_KEY_F6},
  {"f7", GLFW_KEY_F7},
  {"f8", GLFW_KEY_F8},
  {"f9", GLFW_KEY_F9},
  {"f10", GLFW_KEY_F10},
  {"f11", GLFW_KEY_F11},
  {"f12", GLFW_KEY_F12},
  {"f13", GLFW_KEY_F13},
  {"f14", GLFW_KEY_F14},
  {"f15", GLFW_KEY_F15},
  {"f16", GLFW_KEY_F16},
  {"f17", GLFW_KEY_F17},
  {"f18", GLFW_KEY_F18},
  {"f19", GLFW_KEY_F19},
  {"f20", GLFW_KEY_F20},
  {"f21", GLFW_KEY_F21},
  {"f22", GLFW_KEY_F22},
  {"f23", GLFW_KEY_F23},
  {"f24", GLFW_KEY_F24},
  {"f25", GLFW_KEY_F25},
  {"kp_0", GLFW_KEY_KP_0},
  {"kp_1", GLFW_KEY_KP_1},
  {"kp_2", GLFW_KEY_KP_2},
  {"kp_3", GLFW_KEY_KP_3},
  {"kp_4", GLFW_KEY_KP_4},
  {"kp_5", GLFW_KEY_KP_5},
  {"kp_6", GLFW_KEY_KP_6},
  {"kp_7", GLFW_KEY_KP_7},
  {"kp_8", GLFW_KEY_KP_8},
  {"kp_9", GLFW_KEY_KP_9},
  {"kp_decimal", GLFW_KEY_KP_DECIMAL},
  {"kp_divide", GLFW_KEY_KP_DIVIDE},
  {"kp_multiply", GLFW_KEY_KP_MULTIPLY},
  {"kp_subtract", GLFW_KEY_KP_SUBTRACT},
  {"kp_add", GLFW_KEY_KP_ADD},
  {"kp_enter", GLFW_KEY_KP_ENTER},
  {"kp_equal", GLFW_KEY_KP_EQUAL},
  {"left_shift", GLFW_KEY_LEFT_SHIFT},
  {"left_control", GLFW_KEY_LEFT_CONTROL},
  {"left_alt", GLFW_KEY_LEFT_ALT},
  {"left_super", GLFW_KEY_LEFT_SUPER},
  {"right_shift", GLFW_KEY_RIGHT_SHIFT},
  {"right_control", GLFW_KEY_RIGHT_CONTROL},
  {"right_alt", GLFW_KEY_RIGHT_ALT},
  {"right_super", GLFW_KEY_RIGHT_SUPER},
  {"menu", GLFW_KEY_MENU},

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

