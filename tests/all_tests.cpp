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
}
