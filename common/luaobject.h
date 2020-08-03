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

/*
** Entries in the Lua stack
*/
typedef union StackValue {
    TValue val;
} StackValue;


/* index to stack elements */
typedef StackValue *StkId;
#define s2v(o)	(&(o)->val)

#define val_(o)		((o)->value)
#define settt_(o,t)	((o)->tt=(t))

#define setfvalue(o,x) {TValue *io=(o); val_(io).f=(x);settt_(io,LUA_TFUNCTION);}
#define setnvalue(o,x) {TValue *io=(o); val_(io).n=(x);settt_(io,LUA_TNUMBER);}
#define setbvalue(o,x) {TValue *io=(o); val_(io).b=(x);settt_(io,LUA_TBOOLEAN);}
#define setnilvalue(o,x) {TValue *io=(o); val_(io).p=NULL;settt_(io,LUA_TNIL);}
#define setpvalue(o,x) {TValue *io=(o); val_(io).p=(x);settt_(io,LUA_TLIGHTUSERDATA);}
#define setivalue(o,x) {TValue *io=(o); val_(io).i=(x);settt_(io,LUA_NUMINT);}

#endif 