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

class AbstractClass
{
public:
    virtual ~AbstractClass() {}

    virtual void setData(int i) = 0;

private:
    REFLECT_ABSTRACT_CLASS(AbstractClass)
        REFLECT_FUNCTION(setData)
    REFLECT_END()
};
REGISTER_CLASS(AbstractClass)
//----------------------------------------------------------------------------

class DerivedFromAbstractClass : public AbstractClass
{
public:
    virtual ~DerivedFromAbstractClass() {}

    virtual void setData(int i) { m_i = i; }

private:
    int m_i;

    REFLECT_DERIVED_CLASS(DerivedFromAbstractClass,AbstractClass)
    REFLECT_END()
};
REGISTER_CLASS(DerivedFromAbstractClass)
//----------------------------------------------------------------------------

namespace r = reflectos;

TEST_SUITE(Abstract)
{
    TEST(type_inspect)
    {
        EXPECT_EQUAL(r::type_inspect<AbstractClass>::name(), "AbstractClass");
        EXPECT_NOTEQUAL(r::type_inspect<AbstractClass>::id(), 0U);
        EXPECT_FALSE(r::type_inspect<AbstractClass>::isPOD());
        EXPECT_TRUE(r::type_inspect<AbstractClass>::isClass());
        EXPECT_TRUE(r::type_inspect<AbstractClass>::isAbstract());
        EXPECT_TRUE(r::type_inspect<AbstractClass>::isPolymorphic());
        EXPECT_FALSE(r::type_inspect<AbstractClass>::hasSimpleConstructor());
    }

    TEST(Inspect)
    {
        r::TypeInfo const* info = r::inspect("AbstractClass");
        EXPECT_VALID(info);
        EXPECT_EQUAL(info->name(), "AbstractClass");
        EXPECT_FALSE(info->isPOD());
        EXPECT_TRUE(info->isClass());
        EXPECT_TRUE(info->isAbstract());
        EXPECT_TRUE(info->isPolymorphic());
        EXPECT_FALSE(info->hasSimpleConstructor());
    }

    TEST(InspectDerived)
    {
        r::TypeInfo const* info = r::inspect("DerivedFromAbstractClass");
        EXPECT_VALID(info);
        EXPECT_EQUAL(info->name(), "DerivedFromAbstractClass");
        EXPECT_FALSE(info->isPOD());
        EXPECT_TRUE(info->isClass());
        EXPECT_FALSE(info->isAbstract());
        EXPECT_TRUE(info->isPolymorphic());
        EXPECT_TRUE(info->hasSimpleConstructor());

        DerivedFromAbstractClass obj;
        EXPECT_VALID(r::inspect(&obj));
        EXPECT_EQUAL(r::inspect(&obj), info);
    }

    TEST(Construct)
    {
        void* ptr = r::inspect("AbstractClass")->construct();
        EXPECT_EQUAL(ptr, nullptr);
        r::inspect("AbstractClass")->destroy(ptr);
    }

    TEST(ConstructArray)
    {
        void* ptr = r::inspect("AbstractClass")->constructArray(3);
        EXPECT_EQUAL(ptr, nullptr);
        r::inspect("AbstractClass")->destroyArray(ptr);
    }

    TEST(ConstructInplace)
    {
        void* ptr = alloca(r::inspect("AbstractClass")->size());
        r::inspect("AbstractClass")->constructInPlace(ptr);
        r::inspect("AbstractClass")->destroyInPlace(ptr);
    }
}
