#include "luastate.h"
#include "luaobject.h"
#include "luamem.h"

#define fromstate(L) (cast(LX*, cast(lu_byte*, (L)) - offsetof(LX, l)))


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


static void free_stack(lua_State* L) {
    global_State* g = G(L);
    (*g->frealloc)(g->ud, L->stack, sizeof(TValue), 0);
    L->stack = L->stack_last = L->top = NULL;
    L->stack_size = 0;
}

void lua_close(lua_State* L) {
    struct global_State* g = G(L);
    struct lua_State* L1 = g->mainthread; // only mainthread can be close
    
    // because I have not implement gc, so we should free ci manual 
    struct CallInfo* ci = &L1->base_ci;
    while(ci->next) {
        struct CallInfo* next = ci->next->next;
        struct CallInfo* free_ci = ci->next;

        (*g->frealloc)(g->ud, free_ci, sizeof(struct CallInfo), 0);
        ci = next;
    }

    free_stack(L1);    
    (*g->frealloc)(g->ud, fromstate(L1), sizeof(LG), 0);
}

static void stack_init(lua_State* L)
{
    L->stack = luaM_malloc_(L,BASIC_STACK_SIZE*sizeof(StackValue),0);
    L->stack_size = BASIC_STACK_SIZE;
    L->stack_last = L->stack + BASIC_STACK_SIZE - LUA_EXTRASPACE;
    L->next = L->previous = NULL;
    //L->status = LUA_OK;
    L->errorjmp = NULL;
    L->errorfunc= 0;

    for (int i=0;i<L->stack_size;++i)
    {
        setnilvalue(s2v(L->stack +i));
    }
    L->top++;
    L->ci = &L->base_ci;
    L->ci->func = L->top;//???
    L->ci->top = L->top + LUA_MINSTACK;
    L->ci->previous = L->ci->next = NULL;
    L->ci->nresult = 0;
    setnilvalue(s2v(L->top));
    L->ci->callstatus = CIST_C;
}

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

void increase_top(lua_State* L)
{
    L->top++;
}

void lua_pushinteger(struct lua_State* L, int integer)
{
    setivalue(s2v(L->top),integer);
    increase_top(L);
}

void lua_settop(lua_State* L,int index)
{
    //assert(L->top)
    L->top = L->top + index;
}

int lua_gettop(lua_State* L)
{
    //L->top->val
    return 0;
}

void lua_pop(lua_State* L)
{
    lua_settop(L,-1);
}

static StkId index2addr(lua_State* L,int index)
{
    if (index >= 0)
    {
        //func to do
    }
    else
    {
        return L->top + index;
    }
    
}

lua_Integer lua_tointegerx(struct lua_State* L, int idx)
{
    lua_Integer ret = 0;
    TValue* addr = s2v(index2addr(L, idx));
    if (addr->tt == LUA_NUMINT)
    {
        ret = addr->value.i;
    }
    else
    {
        //error to do
        ;
    }
    return ret;
}

CallInfo *luaE_extendCI (lua_State *L) {
    CallInfo *ci;
    //lua_assert(L->ci->next == NULL);
    //luaE_enterCcall(L);
    ci = (CallInfo*)luaM_malloc_(L, sizeof(CallInfo), 0);
    //lua_assert(L->ci->next == NULL);
    L->ci->next = ci;
    ci->previous = L->ci;
    ci->next = NULL;
    //ci->u.l.trap = 0;
    //L->nci++;
    return ci;
}