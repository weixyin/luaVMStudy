#ifndef LUAMEM_H
#define LUAMEM_H

#include "luastate.h"

#define luaM_error(L)	luaD_throw(L, LUA_ERRMEM)

void *luaM_malloc_ (lua_State *L, size_t size, int tag);

#endif