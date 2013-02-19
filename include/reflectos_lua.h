/*=============================================================================

Copyright (c) 2013 Ville Ruusutie

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

=============================================================================*/
#pragma once
#ifndef REFLECTOS_LUA_H
#define REFLECTOS_LUA_H

#include "reflectos.h"
#include "lua.hpp"

namespace reflectos {
//-----------------------------------------------------------------------------

template<typename T>
struct LuaClass
{
    static void regg(lua_State* L)
    {
        lua_newtable(L);
        int methods = lua_gettop(L);

        char const* className = type_inspect<T>::name();
        luaL_newmetatable(L, className);
        int metatable = lua_gettop(L);

        // store method table in globals so that
        // scripts can add functions written in Lua.
        lua_pushstring(L, className);
        lua_pushvalue(L, methods);
        lua_settable(L, LUA_REGISTRYINDEX);

        lua_pushliteral(L, "__metatable");
        lua_pushvalue(L, methods);
        lua_settable(L, metatable);  // hide metatable from Lua getmetatable()

        lua_pushliteral(L, "__index");
        lua_pushvalue(L, methods);
        lua_settable(L, metatable);

        lua_pushliteral(L, "__tostring");
        lua_pushcfunction(L, toString);
        lua_settable(L, metatable);

        lua_pushliteral(L, "__gc");
        lua_pushcfunction(L, garbageCollect);
        lua_settable(L, metatable);

        lua_newtable(L);                // mt for method table
        int mt = lua_gettop(L);
        lua_pushliteral(L, "new");
        lua_pushcfunction(L, construct);
        lua_settable(L, methods);       // add new to method table
        lua_settable(L, mt);            // mt.__call = new_T
        lua_setmetatable(L, methods);

#if 0
        // fill method table with methods from class T
        for (RegType *l = T::methods; l->name; l++) {
            /* edited by Snaily: shouldn't it be const RegType *l ... ? */
            lua_pushstring(L, l->name);
            lua_pushlightuserdata(L, (void*)l);
            lua_pushcclosure(L, thunk, 1);
            lua_settable(L, methods);*/
        }
#endif

        lua_pop(L, 2);  // drop metatable and method table
    }

    static int construct(lua_State* L)
    {
        lua_remove(L, 1);
        void* obj = lua_newuserdata(L, type_inspect<T>::size());
        type_inspect<T>::constructInPlace(obj);
        //luaL_getmetatable(L, type_inspect<T>::name());
        lua_getfield(L, LUA_REGISTRYINDEX, type_inspect<T>::name());
        lua_setmetatable(L, -2);
        return 1;
    }

    static int garbageCollect(lua_State* L)
    {
        void* obj = lua_touserdata(L, 1);
        type_inspect<T>::destroy(obj);
        return 0;
    }

    static int toString(lua_State* L)
    {
        char buffer[32];
        void* obj = lua_touserdata(L, 1);
        sprintf_s(buffer, "%p", obj);
        lua_pushfstring(L, "%s (%s)", type_inspect<T>::name(), buffer);
        return 1;
    }
};


struct LuaBinder
{
    LuaBinder(lua_State* lua) : m_lua(lua) {}
    lua_State* m_lua;

    template<typename T>
    void visitClass()
    {
        LuaClass<T>::regg(m_lua);
    }

    template<typename T,typename F>
    void visitField(const char* name, T const* object, F const* field)
    {
        /*// TODO implement
        FieldInfo info;
        info.m_id = fnv1a_hash(name);
        info.m_name = name;
        info.m_offset = (uint32)((ptrdiff_t)field - (ptrdiff_t)object);
        info.m_next = m_firstField;*/
    }

    template<typename T,typename funcptr_t,funcptr_t Function>
    void visitFunction(const char* name, T const*)
    {
        /*typedef member_function_inspect<T,funcptr_t> Inspection;
        auto funcPtr = &(FunctionPtr<Inspection::Type>::ptr<T,funcptr_t,Function>);
        static FunctionInfoImpl info(name, FunctionInfo::PtrType(funcPtr), &m_firstFunction);*/
    }
};
//-----------------------------------------------------------------------------

} // end of reflectos

#endif
