#ifndef LUADO_H
#define LUADO_H

#include "luastate.h"

int luaD_call(lua_State* L, StkId func, int nresult);
int luaD_precall(lua_State* L, StkId func, int nresult);
int luaD_checkstack(lua_State* L,int size);

int luaD_poscall(lua_State* L, StkId first_result);

#endif
