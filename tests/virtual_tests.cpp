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

class BaseClass
{
public:
    virtual ~BaseClass() {}
    BaseClass() : m_data(0) {}

    int getData() { return m_data; }
    virtual int test() { return m_data; }

protected:
    int m_data;

    REFLECT_CLASS(BaseClass)
        REFLECT_FIELD(m_data)
        REFLECT_FUNCTION(getData)
        REFLECT_FUNCTION(test)
    REFLECT_END()
};
REGISTER_CLASS(BaseClass)
//----------------------------------------------------------------------------

class DerivedClass : public BaseClass
{
public:
    virtual ~DerivedClass() { delete m_temp; }
    DerivedClass() { m_data = 5; m_temp = new int; }

    virtual int test() { return 77; }
    virtual int foo() { return 333; }

public:
    int m_extra;
    int* m_temp;

private:
    REFLECT_VIRTUAL_CLASS(DerivedClass,BaseClass)
        REFLECT_FIELD(m_extra)
        REFLECT_FIELD(m_temp)
        REFLECT_FUNCTION(foo)
    REFLECT_END()
};
REGISTER_CLASS(DerivedClass)
//----------------------------------------------------------------------------

namespace r = reflectos;

REGISTER_TYPE(int)

TEST_SUITE(Virtual)
{
    TEST(InspectBase)
    {
        EXPECT_EQUAL(r::type_inspect<BaseClass>::name(), "BaseClass");
        EXPECT_NOTEQUAL(r::type_inspect<BaseClass>::id(), 0U);
        EXPECT_FALSE(r::type_inspect<BaseClass>::isPOD());
        EXPECT_TRUE( r::type_inspect<BaseClass>::isClass());
        EXPECT_FALSE(r::type_inspect<BaseClass>::isAbstract());
        EXPECT_TRUE(r::type_inspect<BaseClass>::isPolymorphic());
        EXPECT_TRUE( r::type_inspect<BaseClass>::hasSimpleConstructor());
        EXPECT_VALID(r::type_inspect<BaseClass>::getField("m_data"));
        EXPECT_VALID(r::type_inspect<BaseClass>::getFunction("getData"));
        EXPECT_VALID(r::type_inspect<BaseClass>::getFunction("test"));
    }

    TEST(InspectDerived)
    {
        EXPECT_EQUAL(r::type_inspect<DerivedClass>::name(), "DerivedClass");
        EXPECT_NOTEQUAL(r::type_inspect<DerivedClass>::id(), 0U);
        EXPECT_FALSE(r::type_inspect<DerivedClass>::isPOD());
        EXPECT_TRUE( r::type_inspect<DerivedClass>::isClass());
        EXPECT_FALSE(r::type_inspect<DerivedClass>::isAbstract());
        EXPECT_TRUE( r::type_inspect<DerivedClass>::isPolymorphic());
        EXPECT_TRUE( r::type_inspect<DerivedClass>::hasSimpleConstructor());
        EXPECT_EQUAL(r::type_inspect<DerivedClass>::base::id(), r::type_inspect<BaseClass>::id());
        EXPECT_VALID(r::type_inspect<DerivedClass>::getField("m_extra"));
        EXPECT_VALID(r::type_inspect<DerivedClass>::getField("m_temp"));
        EXPECT_VALID(r::type_inspect<DerivedClass>::getFunction("foo"));
    }

    TEST(ConstructBase)
    {
        void* ptr = r::inspect("BaseClass")->construct();
        EXPECT_VALID(ptr);
        BaseClass* basePtr = reinterpret_cast<BaseClass*>(ptr);
        EXPECT_EQUAL(basePtr->getData(), 0);
        EXPECT_EQUAL(basePtr->test(), 0);
        r::inspect("BaseClass")->destroy(ptr);
    }

    TEST(ConstructDerived)
    {
        void* ptr = r::inspect("DerivedClass")->construct();
        EXPECT_VALID(ptr);
        DerivedClass* derivedPtr = reinterpret_cast<DerivedClass*>(ptr);
        EXPECT_EQUAL(derivedPtr->getData(), 5);
        EXPECT_EQUAL(derivedPtr->test(), 77);
        r::inspect("DerivedClass")->destroy(ptr);
    }

    TEST(ConstructDerivedInplace)
    {
        void* ptr = alloca(r::inspect("DerivedClass")->size());
        r::inspect("DerivedClass")->constructInPlace(ptr);
        DerivedClass* derivedPtr = reinterpret_cast<DerivedClass*>(ptr);
        EXPECT_EQUAL(derivedPtr->getData(), 5);
        EXPECT_EQUAL(derivedPtr->test(), 77);
        r::inspect("DerivedClass")->destroyInPlace(ptr);
    }

    TEST(ConstructDerivedDestroyBase)
    {
        void* ptr = r::inspect("DerivedClass")->construct();
        EXPECT_VALID(ptr);
        BaseClass* basePtr = reinterpret_cast<BaseClass*>(ptr);
        EXPECT_EQUAL(basePtr->getData(), 5);
        EXPECT_EQUAL(basePtr->test(), 77);
        r::inspect("BaseClass")->destroy(ptr);
    }

    TODO_TEST(ConstructDerivedInspectBase)
    {
        void* ptr = r::inspect("DerivedClass")->construct();
        EXPECT_VALID(ptr);
        BaseClass* basePtr = reinterpret_cast<BaseClass*>(ptr);
        EXPECT_EQUAL(r::inspect(basePtr)->name(), r::type_inspect<DerivedClass>::name());
        EXPECT_EQUAL(r::inspect(basePtr)->id(), r::type_inspect<DerivedClass>::id());
        r::inspect("DerivedClass")->destroy(ptr);
    }

    TEST(Inspect)
    {
        DerivedClass clss;
        BaseClass* obj = &clss;
        r::TypeInfo const* info = r::inspect(obj);
        EXPECT_VALID(info);
        EXPECT_EQUAL(info->name(), "DerivedClass");
        EXPECT_FALSE(info->isPOD());
        EXPECT_TRUE(info->isClass());
        EXPECT_FALSE(info->isAbstract());
        EXPECT_TRUE(info->isPolymorphic());
    }
}
