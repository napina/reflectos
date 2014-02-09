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
#include "derivedlib/derivedclass.h"

namespace r = reflectos;

TEST_SUITE(DerivedLib)
{
    TEST(Inspect) {
        r::TypeInfo const* info = r::inspect("SharedDerivedClass");
        EXPECT_VALID(info);
        EXPECT_EQUAL(info->name(), "SharedDerivedClass");
        EXPECT_FALSE(info->isPOD());
        EXPECT_TRUE(info->isClass());
        EXPECT_FALSE(info->isAbstract());
        EXPECT_TRUE(info->isPolymorphic());
        EXPECT_TRUE(info->hasSimpleConstructor());
        EXPECT_VALID(info->getField("m_data"));
        EXPECT_VALID(info->getField("m_extra"));
        EXPECT_VALID(info->getField("m_temp"));
        EXPECT_VALID(info->getFunction("getData"));
        EXPECT_VALID(info->getFunction("test"));
        EXPECT_VALID(info->getFunction("foo"));
    }

    TEST(Construct)
    {
        r::TypeInfo const* info = r::inspect("SharedDerivedClass");
        EXPECT_VALID(info);
        void* ptr = info->construct();
        EXPECT_VALID(ptr);
        info->destroy(ptr);
    }

    TEST(ConstructArray)
    {
        r::TypeInfo const* info = r::inspect("SharedDerivedClass");
        EXPECT_VALID(info);
        void* ptr = info->constructArray(3);
        EXPECT_VALID(ptr);
        info->destroyArray(ptr);
    }

    TEST(ConstructInplace)
    {
        r::TypeInfo const* info = r::inspect("SharedDerivedClass");
        EXPECT_VALID(info);
        void* ptr = alloca(info->size());
        info->constructInPlace(ptr);
        info->destroyInPlace(ptr);
    }
}
