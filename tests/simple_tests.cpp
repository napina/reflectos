#include "unitos/unitos.h"
#include "reflectos.h"
#include <malloc.h>

using namespace fastdelegate;
namespace r = reflectos;

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
//----------------------------------------------------------------------------

class Fixture : public unitos::SuiteTest
{
public:
    Fixture() : unitos::SuiteTest()
    {
        SimpleClass::reflect((r::internal::ClassInfoImpl<SimpleClass>*)r::inspect("SimpleClass"), nullptr);
    }

    virtual ~Fixture() {}
};

//----------------------------------------------------------------------------

REGISTER_TYPE(int)

TEST_SUITE(Simple)
{
    TEST(type_inspect)
    {
        EXPECT_EQUAL(r::type_inspect<SimpleClass>::name(), "SimpleClass");
        EXPECT_NOTEQUAL(r::type_inspect<SimpleClass>::id(), 0);
        EXPECT_FALSE(r::type_inspect<SimpleClass>::isPOD());
        EXPECT_TRUE( r::type_inspect<SimpleClass>::isClass());
        EXPECT_FALSE(r::type_inspect<SimpleClass>::isAbstract());
        EXPECT_FALSE(r::type_inspect<SimpleClass>::isPolymorphic());
        EXPECT_TRUE( r::type_inspect<SimpleClass>::hasSimpleConstructor());
    }

    TEST(Inspect)
    {
        SimpleClass obj;

        r::TypeInfo const* info = r::inspect("SimpleClass");
        EXPECT_VALID(info);
        EXPECT_EQUAL(info->name(), "SimpleClass");
        EXPECT_EQUAL(info->id(), r::type_inspect<SimpleClass>::id());
        EXPECT_FALSE(info->isPOD());
        EXPECT_TRUE( info->isClass());
        EXPECT_FALSE(info->isAbstract());
        EXPECT_FALSE(info->isPolymorphic());
        EXPECT_TRUE( info->hasSimpleConstructor());

        EXPECT_VALID(r::inspect(obj));
        EXPECT_EQUAL(r::inspect(obj), info);
    }

    TEST(Construct)
    {
        void* ptr = r::inspect("SimpleClass")->construct();
        EXPECT_VALID(ptr);
        r::inspect("SimpleClass")->destroy(ptr);
    }

    TEST(ConstructArray)
    {
        void* ptr = r::inspect("SimpleClass")->constructArray(3);
        EXPECT_VALID(ptr);
        r::inspect("SimpleClass")->destroyArray(ptr);
    }

    TEST(ConstructInplace)
    {
        void* ptr = alloca(r::inspect("SimpleClass")->size());
        r::inspect("SimpleClass")->constructInPlace(ptr);
        r::inspect("SimpleClass")->destroyInPlace(ptr);
    }

    TEST_FIXTURE(Fixture,CallGetData)
    {
        SimpleClass c;
        c.setData(98);
        r::FunctionInfo const* info = r::inspect("SimpleClass")->getFunction("getData");
        EXPECT_VALID(info);
        int value = info->call0<int>(&c);
        EXPECT_EQUAL(value, 98);

        info = r::inspect("SimpleClass")->getFunction("one");
        EXPECT_VALID(info);
        value = info->call0<int>(&c);
        EXPECT_EQUAL(value, 1);
    }

	/*TEST_FIXTURE(Fixture,CallOne)
    {
        SimpleClass c;
        r::FunctionInfo const*info = r::inspect("SimpleClass")->getFunction("one");
        EXPECT_VALID(info);
        int value = info->call0<int>(&c);
        EXPECT_EQUAL(value, 1);
    }*/
}
