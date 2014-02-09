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

#pragma warning(disable:4505)

namespace {

struct Class
{
    void none(int, const char*) {}
    int one() { return 1; }
    long two() const { return 2L; }
    static char* test() { return "test"; }
};

static int globalFunc(int value,const char*,long) { return value; }

}
//----------------------------------------------------------------------------

REGISTER_TYPE(int)

namespace r = reflectos;

#define EXPECT_SAME(Type,ExpectedType)\
    EXPECT_TRUE((r::is_same<Type,ExpectedType>::value));

TEST_SUITE(Function)
{
    TEST(None)
    {
        typedef r::member_function_inspect<Class,decltype(&Class::none)> info;
        EXPECT_EQUAL(info::count, 2);
        EXPECT_FALSE(info::returns);
        EXPECT_FALSE(info::is_const);
        EXPECT_FALSE(info::is_static);
        EXPECT_FALSE(info::is_global);
        EXPECT_SAME(info::R, void);
        EXPECT_SAME(info::A, int);
        EXPECT_SAME(info::B, const char*);
    }

    TEST(One)
    {
        typedef r::member_function_inspect<Class,decltype(&Class::one)> info;
        EXPECT_EQUAL(info::count, 0);
        EXPECT_TRUE(info::returns);
        EXPECT_FALSE(info::is_const);
        EXPECT_FALSE(info::is_static);
        EXPECT_FALSE(info::is_global);
        EXPECT_SAME(info::R, int);
    }

    TEST(Two)
    {
        typedef r::member_function_inspect<Class,decltype(&Class::two)> info;
        EXPECT_EQUAL(info::count, 0);
        EXPECT_TRUE(info::returns);
        EXPECT_TRUE(info::is_const);
        EXPECT_FALSE(info::is_static);
        EXPECT_FALSE(info::is_global);
        EXPECT_SAME(info::R, long);
    }

    TEST(Test)
    {
        typedef r::member_function_inspect<Class,decltype(&Class::test)> info;
        EXPECT_EQUAL(info::count, 0);
        EXPECT_TRUE(info::returns);
        EXPECT_FALSE(info::is_const);
        EXPECT_TRUE(info::is_static);
        EXPECT_FALSE(info::is_global);
        EXPECT_SAME(info::R, char*);
    }

    TEST(Global)
    {
        typedef r::function_inspect<decltype(&globalFunc)> info;
        EXPECT_EQUAL(info::count, 3);
        EXPECT_TRUE(info::returns);
        EXPECT_FALSE(info::is_const);
        EXPECT_FALSE(info::is_static);
        EXPECT_TRUE(info::is_global);
        EXPECT_SAME(info::R, int);
        EXPECT_SAME(info::A, int);
        EXPECT_SAME(info::B, const char*);
        EXPECT_SAME(info::C, long);
    }
}
