#ifndef LUAAUX_H
#define LUAAUX_H

#include "lua.h"

struct lua_State* luaL_newstate();
void luaL_close(lua_State* L);

#endif