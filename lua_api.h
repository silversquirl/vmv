#ifndef __VMV_LUA_API_H__
#define __VMV_LUA_API_H__

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

extern int visualiser_ref;

void init_api(lua_State *L);
int run_visualisation(lua_State *L, unsigned int *bars, int length);

#endif
