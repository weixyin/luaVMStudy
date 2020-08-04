#ifndef LUAAUX_H
#define LUAAUX_H

#include "lua.h"

struct lua_State* luaL_newstate();
void luaL_close(lua_State* L);

lua_Integer luaL_tointeger(struct lua_State* L, int idx);

#endif