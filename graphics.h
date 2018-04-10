#ifndef __VMV_GRAPHICS_H__
#define __VMV_GRAPHICS_H__

#include <lua.h>
#include <lualib.h>
#include "lua_config.h"

struct monitor_info {
  unsigned width, height, refresh_rate;
};

int mainloop(struct config *config);
int get_monitor_details(int id, struct monitor_info *info); // id is -1 for default monitor

#endif
