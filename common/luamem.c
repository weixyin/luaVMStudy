#include "luamem.h"

void* luaM_malloc_(lua_State* L,size_t size,int tag)
{
    if(size == 0)
        return NULL;

    global_State *g = G(L);
    void* newblock = ((*g->frealloc)(g->ud, NULL, tag, size));
    //if(newblock == NULL)
    //    luaM_error(L);
    return newblock;
}