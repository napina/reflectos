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
#include "unitos/unitos.h"
#include "reflectos.h"

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
REGISTER_CLASS(SimpleClass, 0)
//----------------------------------------------------------------------------

REGISTER_TYPE(int)

TEST_SUITE(Simple)
{
    TEST(type_inspect)
    {
        EXPECT_EQUAL(r::type_inspect<SimpleClass>::name(), "SimpleClass");
        EXPECT_NOTEQUAL(r::type_inspect<SimpleClass>::id(), 0U);
        EXPECT_FALSE(r::type_inspect<SimpleClass>::isPOD());
        EXPECT_TRUE( r::type_inspect<SimpleClass>::isClass());
        EXPECT_FALSE(r::type_inspect<SimpleClass>::isAbstract());
        EXPECT_FALSE(r::type_inspect<SimpleClass>::isPolymorphic());
        EXPECT_TRUE( r::type_inspect<SimpleClass>::hasSimpleConstructor());
        EXPECT_VALID(r::type_inspect<SimpleClass>::getField("m_data"));
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
        EXPECT_VALID(info->getField("m_data"));

        EXPECT_VALID(r::inspect(&obj));
        EXPECT_EQUAL(r::inspect(&obj), info);
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

    TEST(CallGetData)
    {
        SimpleClass c;
        c.setData(98);
        r::FunctionInfo const* info = r::inspect("SimpleClass")->getFunction("getData");
        EXPECT_VALID(info);
        int value = info->call<int>(&c);
        EXPECT_EQUAL(value, 98);
    }

    TEST(CallSetData)
    {
        SimpleClass c;
        r::FunctionInfo const* info = r::inspect("SimpleClass")->getFunction("setData");
        EXPECT_VALID(info);
        info->call<void,int>(&c,33);
        EXPECT_EQUAL(c.getData(), 33);
    }

    TEST(CallOne)
    {
        SimpleClass c;
        r::FunctionInfo const*info = r::inspect("SimpleClass")->getFunction("one");
        EXPECT_VALID(info);
        int value = info->call<int>(&c);
        EXPECT_EQUAL(value, 1);
    }
}
