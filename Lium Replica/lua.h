#pragma once


struct lua_State;

using lua_CFunction = int(*)(lua_State*);
inline void lua_Execute(const char* text)
{
    ((void(__cdecl*)(const char*, const char*, void*))0x00819210)(text, "\0", nullptr);
}

inline auto lua_Register(const char* command, int func_pointer)
{
    return ((bool(__cdecl*)(const char*, int))0x00817F90)(command, func_pointer);
}

inline int lua_isstring(lua_State* L, int idx)
{
    return ((decltype(&lua_isstring))0x0084DF60)(L, idx);
}

inline char* ToString(lua_State* state, int n_param)
{
    return ((char* (__cdecl*)(lua_State*, int, int))0x0084E0E0)(state, n_param, 0);
}

inline int RunProtectedFunction(lua_State* L)
{

    if (lua_isstring(L, 1))
    {
        lua_Execute(ToString(L, 1));
    }
    return 0;
}