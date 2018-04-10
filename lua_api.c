#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "lua_api.h"

// triangle({x, y}, {x, y}, {x, y}, {r, g, b})
static int triangle(lua_State *lua) {
  if (lua_gettop(lua) != 4)
    return 0;

  glBegin(GL_TRIANGLES);

  lua_rawgeti(lua, 4, 1);
  double r = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 4, 2);
  double g = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 4, 3);
  double b = lua_tonumber(lua, -1);

  glColor3f(r, g, b);

  double x, y;

  lua_rawgeti(lua, 1, 1);
  x = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 1, 2);
  y = lua_tonumber(lua, -1);

  glVertex2f(x, y);

  lua_rawgeti(lua, 2, 1);
  x = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 2, 2);
  y = lua_tonumber(lua, -1);

  glVertex2f(x, y);

  lua_rawgeti(lua, 3, 1);
  x = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 3, 2);
  y = lua_tonumber(lua, -1);

  glVertex2f(x, y);

  glEnd();

  return 0;
}

// rectangle({x, y}, {x, y}, {x, y}, {x, y}, {r, g, b})
static int rectangle(lua_State *lua) {
  if (lua_gettop(lua) != 5)
    return 0;

  glBegin(GL_TRIANGLES);

  lua_rawgeti(lua, 5, 1);
  double r = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 5, 2);
  double g = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 5, 3);
  double b = lua_tonumber(lua, -1);

  glColor3f(r, g, b);

  lua_rawgeti(lua, 1, 1);
  double x1 = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 1, 2);
  double y1 = lua_tonumber(lua, -1);

  lua_rawgeti(lua, 2, 1);
  double x2 = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 2, 2);
  double y2 = lua_tonumber(lua, -1);

  lua_rawgeti(lua, 3, 1);
  double x3 = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 3, 2);
  double y3 = lua_tonumber(lua, -1);

  lua_rawgeti(lua, 4, 1);
  double x4 = lua_tonumber(lua, -1);
  lua_rawgeti(lua, 4, 2);
  double y4 = lua_tonumber(lua, -1);

  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glVertex2f(x3, y3);

  glVertex2f(x1, y1);
  glVertex2f(x3, y3);
  glVertex2f(x4, y4);

  glEnd();

  return 0;
}

struct func_pair {
  const char *name;
  lua_CFunction f;
};

const struct func_pair draw[] = {
  { "triangle", triangle },
  { "rectangle", rectangle },
  { NULL },
};

void init_api(struct config *config) {
  lua_newtable(config->lua);

  // Drawing API
  lua_newtable(config->lua);
  for (const struct func_pair *fp = draw; fp->name; ++fp) {
    lua_pushcfunction(config->lua, fp->f);
    lua_setfield(config->lua, -2, fp->name);
  }
  lua_setfield(config->lua, -2, "draw");

  // Misc
  lua_pushinteger(config->lua, BAR_MAX);
  lua_setfield(config->lua, -2, "bar_max");

  lua_setglobal(config->lua, "vmv");
}

int run_visualisation(struct config *config, struct buffer bars) {
  lua_rawgeti(config->lua, LUA_REGISTRYINDEX, config->visualiser_ref);
  lua_newtable(config->lua);

  for (int i = 0; i < bars.len; i++) {
    lua_pushinteger(config->lua, i+1);
    lua_pushinteger(config->lua, bars.buf[i]);
    lua_settable(config->lua, -3);
  }

  if (lua_pcall(config->lua, 1, 0, 0))
    return -1;

  return 0;
}

void set_visualiser(struct config *config) {
  config->visualiser_ref = luaL_ref(config->lua, LUA_REGISTRYINDEX);
}
