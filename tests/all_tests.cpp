// Copyright (C) Ville Ruusutie, 2011

#include "unitos/unitos.h"
#include "reflectos.h"
#include <string>

namespace r = reflectos;

TEST_SUITE(Registry)
{
    TEST(IterateAll)
    {
		const r::TypeInfo* typeInfo = r::internal::Registry::s_typeList;
        while( typeInfo != nullptr )
        {
            printf( "%s\n", typeInfo->name() );
			const r::FunctionInfo* funcInfo = typeInfo->getFirstFunction();
            while( funcInfo != nullptr )
            {
                printf( "  %s(", funcInfo->name() );
                /*for( size_t i = 0; i < funcInfo->getParameterCount(); ++i )
                {
                    const ParameterInfo& param = funcInfo->getParameter(i);
                    printf( "%s", param.typeName );
                    if( i < (funcInfo->getParameterCount() - 1) )
                        printf( ", " );
                }*/
                printf( ")\n" );
				funcInfo = funcInfo->next();
            }
            typeInfo = typeInfo->next();
        }
    }
#if 0
    TEST(CreateDelete)
    {
        //ObjectPtr object = ReflectionDB::createObject("Namespace::SimpleClass");
    }

    TEST(InvokeAdd)
    {
        const FunctionInfo* func = Registry::getClass("Namespace::SimpleClass")->getFunction("add");
        int ret = func->invoke<int (int, int)>(nullptr)(3, 4);
        EXPECT_EQUAL(ret, 7);
    }

    TEST(InvokeGetData)
    {
        Namespace::SimpleClass obj;
        obj.setData(87);

        const FunctionInfo* func = obj.getClassInfo()->getFunction("getData");
        int ret = func->invoke<int (void)>(&obj)();
        EXPECT_EQUAL(ret, 87);
    }

    TEST(InvokeSetData)
    {
        Namespace::SimpleClass obj;
        obj.setData(0);

        const FunctionInfo* func = obj.getClassInfo()->getFunction("setData");
        func->invoke<void (int)>(&obj)(99);
        EXPECT_EQUAL(obj.getData(), 99);
    }
#endif
}
