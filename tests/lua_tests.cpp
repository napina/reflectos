// Copyright (c) 2013 Ville Ruusutie
#include "unitos/unitos.h"
#if 0
#include "reflectos_lua.h"

namespace r = reflectos;
//----------------------------------------------------------------------------

namespace {

void* allocate(void* userdata, void* ptr, size_t oldSize, size_t newSize)
{
    userdata;
    oldSize;
    if(newSize == 0) {
        ::free(ptr);
        return nullptr;
    }
    return ::realloc(ptr, newSize);
}

}
//----------------------------------------------------------------------------

class LuaClass
{
public:
    LuaClass() : m_data(3) {}

    void setData(int i) { m_data = i; }
    int getData() const { return m_data; }
    int one() const { return 1; }

private:
    int m_data;

    REFLECT_CLASS(LuaClass)
        REFLECT_FIELD(m_data)
        REFLECT_FUNCTION(setData)
        REFLECT_FUNCTION(getData)
        REFLECT_FUNCTION(one)
    REFLECT_END()
};

REGISTER_CLASS(LuaClass)
//----------------------------------------------------------------------------

struct LuaFixture : public unitos::SuiteTest
{
    LuaFixture()
    {
        L = lua_newstate(::allocate, nullptr);
        r::LuaBinder luaBinder(L);
        luaBinder.visitClass<LuaClass>();
    }

    virtual ~LuaFixture()
    {
        lua_close(L);
    }

    lua_State* L;
};

TEST_SUITE(Lua)
{
    TEST_FIXTURE(LuaFixture,GetData)
    {
        
    }
}
#endif
