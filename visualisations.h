#include <lua.h>

int prepare_visualiser(const char *filename, lua_State **L);
int run_visualisation(lua_State *L, int length, unsigned int *points, int *vertex_count, double **vertices);
