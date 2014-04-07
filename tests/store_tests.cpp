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
#include <string>

//----------------------------------------------------------------------------
struct SubType
{
    SubType() : m_float(0.0f) {}

    float m_float;

    REFLECT_CLASS(SubType)
        REFLECT_FIELD(m_float)
    REFLECT_END()
};
REGISTER_CLASS(SubType, 0)
//----------------------------------------------------------------------------

class StoredClass
{
public:
    StoredClass() : m_int(0), m_subType() {}

    int     m_int;
    SubType m_subType;

    REFLECT_CLASS(StoredClass)
        REFLECT_FIELD(m_int)
        REFLECT_FIELD(m_subType)
    REFLECT_END()
};
REGISTER_CLASS(StoredClass, 0)
//----------------------------------------------------------------------------

class Store
{
public:
    template<typename T,typename F>
    void visitField(const char* name, T const* object, F const* field)
    {
        name;
        object;
        if(r::type_inspect<F>::isPOD()) {
            //printf("%s %s\n", r::type_inspect<F>::name(), name);
        } else {
            //printf("%s %s\n", r::type_inspect<F>::name(), name);
            r::reflect(this, field);
        }
    }

    template<typename T,typename funcptr_t,funcptr_t Function>
    void visitFunction(const char* name, T const*) {}
};
//----------------------------------------------------------------------------

REGISTER_TYPE(float)

namespace r = reflectos;

TEST_SUITE(Store)
{
    TEST(Test)
    {
        Store store;

        StoredClass storedClass;

        StoredClass::reflect(&store, &storedClass);
    }
}
