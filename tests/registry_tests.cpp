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

namespace r = reflectos;

TEST_SUITE(Registry)
{
#if 1
    TEST(IterateAll)
    {
        const r::TypeInfo* typeInfo = r::Registry::s_typeList;
        while(typeInfo != nullptr) {
            unitos::String str(2048);
            str << "type ";
            str << typeInfo->name();
            if(typeInfo->base() != nullptr) {
                str << " : " << typeInfo->base()->name();
            }
            str << '\n';

            const r::FieldInfo* fieldInfo = typeInfo->getFirstField();
            while(fieldInfo != nullptr) {
                const r::TypeInfo* fieldTypeInfo = fieldInfo->type();
                if(fieldTypeInfo)
                    str << "  " << fieldTypeInfo->name() << ' ' << fieldInfo->name() << '\n';
                else
                    str << "  unknown " << fieldInfo->name() << '\n';
                fieldInfo = fieldInfo->next();
            }

            const r::FunctionInfo* funcInfo = typeInfo->getFirstFunction();
            while(funcInfo != nullptr) {
                str << "  void " << funcInfo->name() << '(';
#if 0
                for( size_t i = 0; i < funcInfo->getParameterCount(); ++i )
                {
                    const ParameterInfo& param = funcInfo->getParameter(i);
                    printf( "%s", param.typeName );
                    if( i < (funcInfo->getParameterCount() - 1) )
                        printf( ", " );
                }
#endif
                str << ")\n";
                funcInfo = funcInfo->next();
            }
            str.terminate();
            *this << str;
            typeInfo = typeInfo->next();
        }
    }
#endif
}
