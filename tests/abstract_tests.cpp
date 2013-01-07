#include "unitos/unitos.h"
#include "reflectos.h"

class AbstractClass
{
public:
    AbstractClass() {}

    virtual void setData(int i) = 0;

private:
    REFLECT_CLASS(AbstractClass)
        REFLECT_FUNCTION(setData)
    REFLECT_END()
};
REGISTER_CLASS(AbstractClass)
//----------------------------------------------------------------------------

namespace r = reflectos;

TEST_SUITE(Abstract)
{
    TEST(type_inspect)
    {
        EXPECT_EQUAL(r::type_inspect<AbstractClass>::name(), "AbstractClass");
        EXPECT_NOTEQUAL(r::type_inspect<AbstractClass>::id(), 0);
        EXPECT_FALSE(r::type_inspect<AbstractClass>::isPOD());
        EXPECT_TRUE(r::type_inspect<AbstractClass>::isClass());
        EXPECT_TRUE(r::type_inspect<AbstractClass>::isAbstract());
        EXPECT_TRUE(r::type_inspect<AbstractClass>::isPolymorphic());
        EXPECT_FALSE(r::type_inspect<AbstractClass>::hasSimpleConstructor());
    }

    TEST(Inspect)
    {
        AbstractClass* obj = nullptr;

        r::TypeInfo const* info = r::inspect("AbstractClass");
        EXPECT_VALID(info);
        EXPECT_EQUAL(info->name(), "AbstractClass");
        EXPECT_FALSE(info->isPOD());
        EXPECT_TRUE(info->isClass());
        EXPECT_TRUE(info->isAbstract());
        EXPECT_TRUE(info->isPolymorphic());
        EXPECT_FALSE(info->hasSimpleConstructor());

        EXPECT_VALID(r::inspect(obj));
        EXPECT_EQUAL(r::inspect(obj), info);
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
