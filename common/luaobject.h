#ifndef LUAOBJECT_H
#define LUAOBJECT_H 

#include "lua.h"

typedef struct lua_State lua_State;

typedef LUA_INTEGER lua_Integer;
typedef LUA_NUMBER lua_Number;
typedef unsigned char lu_byte;
typedef int (*lua_CFunction)(lua_State* L);

typedef union lua_Value
{
    void* p;
    int b;
    lua_Integer i;
    lua_Number n;
    lua_CFunction f;
}Value;

typedef struct lua_TValue
{
    Value value;
    int tt;
}TValue;



#endif 