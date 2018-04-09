#include <stdio.h>
#include <stdlib.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "visualisations.h"

/*
 * A Lua visualiser is expected to have a visualise() function.
 * It should take one argument, an array of magnitudes.
 * It should return an array containing vertices to create triangles.
 * The return array should be in the form [x, y, x, y] etc.
 */

int prepare_visualiser(const char *filename, lua_State **L) {
  *L = luaL_newstate();
  if (!L)
    return -1;

  luaL_openlibs(*L);
  
  if (luaL_loadfile(*L, filename))
    return -1;

  if (lua_pcall(*L, 0, 0, 0)) // Priming run - will run top-level code, of which there probably won't be any
    return -1;

  return 0;
}

#define dispose(L) lua_close(L);

int run_visualisation(lua_State *L, int length, unsigned int *points, int *vertex_count, double **vertices) {
  lua_getglobal(L, "visualise"); // Runs visualise() in the Lua code
  lua_newtable(L);

  for (int i = 0; i < length; i++) {
    lua_pushinteger(L, i+1);
    lua_pushinteger(L, points[i]);
    lua_settable(L, -3);
  }
  
  if (lua_pcall(L, 1, 1, 0))
    return -1;

  lua_len(L, -1);
  int len = lua_tonumber(L, -1);
  lua_pop(L, 1);

  if (len % 2 == 1) // x without a y!
    return -1;

  *vertex_count = len / 2;

  *vertices = malloc(len * sizeof(double));

  lua_pushnil(L); // Start at beginning of table
  unsigned int index = 0;
  while (lua_next(L, -2)) { // Read value from table starting just below top of stack
    (*vertices)[index++] = lua_tonumber(L, -1); // Retreive value from table
    lua_pop(L, 1); // Pop value from top of table, putting key on top
  }

  return 0;
}

