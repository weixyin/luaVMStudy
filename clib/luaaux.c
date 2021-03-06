#include "luaaux.h"
#include "luastate.h"

static void* l_alloc(void*ud,void*ptr,size_t osize,size_t nsize)
{
    //(void)ud; ???
    //(void)osize; ???
    if(nsize == 0)
    {
        free(ptr);
        return NULL;
    }
    return realloc(ptr,nsize);
}

struct lua_State* luaL_newstate()
{
    struct lua_State* L = lua_newstate(&l_alloc,NULL);
    return L;
}

void luaL_close(lua_State* L)
{
    lua_close(L);
}

lua_Integer luaL_tointeger(lua_State* L, int index)
{
    lua_Integer ret = lua_tointegerx(L,index);
    return ret;
}
