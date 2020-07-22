#include "luastate.h"

typedef struct LX
{
    lu_byte extra[LUA_EXTRASPACE];//保留空间，防止栈溢出覆盖其它内存块
    lua_State l;
}LX;

typedef struct LG
{
    LX l;
    global_State g;
}LG;//lua_State和global_State绑一起避免内存碎片，减少重新分配时间


lua_State* lua_newstate(lua_Alloc alloc,void* ud)
{
    global_State* g;
    lua_State* L;
    LG* lg = (LG*)(*alloc)(ud,NULL,LUA_TTHREAD,sizeof(LG));
    if (!lg) {
        return NULL;
    }
    g = &lg->g;
    g->ud = ud;
    g->frealloc = alloc;
    g->panic = NULL;

    L = &lg->l.l;
    G(L) = g;
    g->mainthread = L;
    stack_init(L);

    return L;
    
};

static void stack_init(lua_State* L)
{
    L->stack = (StkId)luaM_realloc(L,NULL,0,LUA_STACKSIZE*sizeof(TValue));
    L->stack_size = LUA_STACKSIZE;
    L->stack_last = L->stack + Lua_STACKSIZE - LUA_EXTRASPACE;
    L->next = L->previous = NULL;
    L->status = LUA_OK;
    L->errorjmp = NULL;
    L->errorfunc= 0;

    int i;
    for (i=0;i<L->stack_size;++i)
    {
        setnilvalue(L->stack +i);
    }
    L->top++;
    L->ci = &L->base_ci;
    L->ci->func = L->stack;//???
    L->ci->top = L->stack + LUA_MINSTACK;
    L->ci->previous = L->ci->next = NULL;
}
