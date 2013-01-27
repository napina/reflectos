#include "unitos/unitos.h"
#include "reflectos.h"

struct Class
{
    void none(int, const char*) {}
    int one() { return 1; }
    long two() const { return 2L; }
    static char* test() { return "test"; }
};

static int global(int value,const char*,long) { return value; }

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
        typedef r::function_inspect<decltype(&global)> info;
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
