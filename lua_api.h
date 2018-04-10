#ifndef __VMV_LUA_API_H__
#define __VMV_LUA_API_H__

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include "audio.h"
#include "lua_config.h"

void init_api(struct config *config);
int run_visualisation(struct config *config, struct buffer bars);
void set_visualiser(struct config *config);

#endif
