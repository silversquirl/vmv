#ifndef __VMV_LUA_CONFIG_H__
#define __VMV_LUA_CONFIG_H__

#include <lua.h>
#include <lualib.h>

int init_lua(const char *config_file, lua_State **setL);

#endif
