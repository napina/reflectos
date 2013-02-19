#include "unitos/unitos.h"
#include "reflectos_lua.h"

namespace r = reflectos;

namespace {

class SimpleClass
{
public:
    SimpleClass() : m_data(3) {}

    void setData(int i) { m_data = i; }
    int getData() const { return m_data; }
    int one() const { return 1; }

private:
    int m_data;

    REFLECT_CLASS(SimpleClass)
        REFLECT_FIELD(m_data)
        REFLECT_FUNCTION(setData)
        REFLECT_FUNCTION(getData)
        REFLECT_FUNCTION(one)
    REFLECT_END()
};
REGISTER_CLASS(SimpleClass)

}
//----------------------------------------------------------------------------

TEST_SUITE(Lua)
{
    TEST(type_inspect)
    {
		lua_State *L = lua_open();

		lua_close(L);
    }
}
