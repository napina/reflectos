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
        while(typeInfo != nullptr) {
            unitos::String str(2048);
            str << typeInfo->name();
            if(typeInfo->base() != nullptr) {
                str << " : " << typeInfo->base()->name();
            }
            str << '\n';
            const r::FunctionInfo* funcInfo = typeInfo->getFirstFunction();
            while(funcInfo != nullptr) {
                str << "  void " << funcInfo->name() << '(';
                /*for( size_t i = 0; i < funcInfo->getParameterCount(); ++i )
                {
                    const ParameterInfo& param = funcInfo->getParameter(i);
                    printf( "%s", param.typeName );
                    if( i < (funcInfo->getParameterCount() - 1) )
                        printf( ", " );
                }*/
                str << ")\n";
                funcInfo = funcInfo->next();
            }
            str.Terminate();
            *this << str;
            typeInfo = typeInfo->next();
        }
    }
}
