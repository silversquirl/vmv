#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "audio.h"
#include "lua_api.h"

int visualiser_ref = 0;

/* triangle({x, y}, {x, y}, {x, y}, {r, g, b}) */
static int triangle(lua_State *L) {
  if (lua_gettop(L) != 4)
    return 0;

  glBegin(GL_TRIANGLES);

  lua_rawgeti(L, 4, 1);
  double r = lua_tonumber(L, -1);
  lua_rawgeti(L, 4, 2);
  double g = lua_tonumber(L, -1);
  lua_rawgeti(L, 4, 3);
  double b = lua_tonumber(L, -1);

  glColor3f(r, g, b);

  double x, y;

  lua_rawgeti(L, 1, 1);
  x = lua_tonumber(L, -1);
  lua_rawgeti(L, 1, 2);
  y = lua_tonumber(L, -1);

  glVertex2f(x, y);

  lua_rawgeti(L, 2, 1);
  x = lua_tonumber(L, -1);
  lua_rawgeti(L, 2, 2);
  y = lua_tonumber(L, -1);

  glVertex2f(x, y);

  lua_rawgeti(L, 3, 1);
  x = lua_tonumber(L, -1);
  lua_rawgeti(L, 3, 2);
  y = lua_tonumber(L, -1);

  glVertex2f(x, y);

  glEnd();

  return 0;
}

/* rectangle({x, y}, {x, y}, {x, y}, {x, y}, {r, g, b}) */
static int rectangle(lua_State *L) {
  if (lua_gettop(L) != 5)
    return 0;

  glBegin(GL_TRIANGLES);

  lua_rawgeti(L, 5, 1);
  double r = lua_tonumber(L, -1);
  lua_rawgeti(L, 5, 2);
  double g = lua_tonumber(L, -1);
  lua_rawgeti(L, 5, 3);
  double b = lua_tonumber(L, -1);

  glColor3f(r, g, b);

  lua_rawgeti(L, 1, 1);
  double x1 = lua_tonumber(L, -1);
  lua_rawgeti(L, 1, 2);
  double y1 = lua_tonumber(L, -1);

  lua_rawgeti(L, 2, 1);
  double x2 = lua_tonumber(L, -1);
  lua_rawgeti(L, 2, 2);
  double y2 = lua_tonumber(L, -1);

  lua_rawgeti(L, 3, 1);
  double x3 = lua_tonumber(L, -1);
  lua_rawgeti(L, 3, 2);
  double y3 = lua_tonumber(L, -1);

  lua_rawgeti(L, 4, 1);
  double x4 = lua_tonumber(L, -1);
  lua_rawgeti(L, 4, 2);
  double y4 = lua_tonumber(L, -1);

  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glVertex2f(x3, y3);

  glVertex2f(x1, y1);
  glVertex2f(x3, y3);
  glVertex2f(x4, y4);

  glEnd();

  return 0;
}

void init_api(lua_State *L) {
  lua_newtable(L); /* vmv */

  lua_newtable(L); /* vmv.draw */

  lua_pushcfunction(L, triangle); /* vmv.draw.triangle */
  lua_setfield(L, -2, "triangle");
  lua_pushcfunction(L, rectangle); /* vmv.draw.rectangle */
  lua_setfield(L, -2, "rectangle"); 

  lua_setfield(L, -2, "draw"); /* vmv.draw */

  lua_pushinteger(L, BAR_MAX);
  lua_setfield(L, -2, "bar_max"); /* vmv.bar_max */

  lua_setglobal(L, "vmv"); /* vmv */
}

int run_visualisation(lua_State *L, unsigned int *bars, int length) {
  lua_rawgeti(L, LUA_REGISTRYINDEX, visualiser_ref);
  lua_newtable(L);

  for (int i = 0; i < length; i++) {
    lua_pushinteger(L, i+1);
    lua_pushinteger(L, bars[i]);
    lua_settable(L, -3);
  }

  if (lua_pcall(L, 1, 0, 0))
    return -1;

  return 0;
}

