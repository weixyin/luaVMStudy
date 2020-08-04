#include "luado.h"

int luaD_call(lua_State* L, StkId func, int nresult)
{
    if(++L->ncalls > LUA_MAXCALLS)
    {
        //to do

    }

    if(luaD_precall(L,func,nresult) == 0)
    {

    }
}

#define next_ci(L)  (L->ci->next ? L->ci->next : luaE_extendCI(L))


int luaD_precall(lua_State* L, StkId func, int nresult)
{
    switch (func->val.tt)
    {
    case LUA_TLCF:
        /* Lua function */
        lua_CFunction f = func->val.value.f;
        //ptrdiff_t func_diff = savestack(L, func);
        luaD_checkstack(L, LUA_MINSTACK);           // 检查lua_State的空间是否充足，如果不充足，则需要扩展
        //func = restorestack(L, func_diff);
        CallInfo *ci = next_ci(L); 
        L->ci = ci;
        ci->nresult = nresult;
        ci->callstatus = 0;
        ci->func = func;
        ci->top = L->top + LUA_MINSTACK;
        L->ci = ci;
        int n = (*f)(L); //函数实际调用
        if(n<0)
        {
            return 2;//调用失败
        }
        else
        {
            luaD_poscall(L, L->top - n);             // 处理返回值
            return 1;
        }
        
        break;
    
    default:
        break;
    }
    return 0;
}

int luaD_checkstack(lua_State* L,int size)
{
    //首先会检查一下lua_State的栈是否空间充足，如果新创建的CallInfo的top指针，不能在lua_State栈的有效范围之内，那么栈就要扩充，通常是扩充为原来的两倍
    //to do
    return 1;
}

int luaD_poscall(lua_State* L, StkId first_result)
{
    CallInfo* ci = L->ci--;
    StkId ret = ci->func;
    int wanted = ci->nresult;
    int i;
    for (i=wanted;i!=0&&first_result<L->top;i--)
    {
        setObj(L,ret++,first_result++);
    }
    while (i-->0)
    {
        setnilvalue(ret++);
    }
    
    L->top = ret;
    return (wanted-LUA_MULTRET);
}

