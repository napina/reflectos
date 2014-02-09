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
#ifndef DERIVED_CLASS_H
#define DERIVED_CLASS_H
#include "baselib/baseclass.h"
#include "reflectos.h"

#ifdef BUILD_DERIVED_DLL
#define DERIVED_DLL __declspec(dllexport)
#else
#define DERIVED_DLL __declspec(dllimport)
#endif

class SharedDerivedClass : public SharedBaseClass
{
public:
    DERIVED_DLL virtual ~SharedDerivedClass();
    DERIVED_DLL SharedDerivedClass();

    DERIVED_DLL virtual int test();
    DERIVED_DLL virtual int foo();

public:
    int m_extra;
    float* m_temp;

    REFLECT_VIRTUAL_CLASS(SharedDerivedClass, SharedBaseClass)
        REFLECT_FIELD(m_extra)
        REFLECT_FIELD(m_temp)
        REFLECT_FUNCTION(foo)
    REFLECT_END()
};

DERIVED_DLL reflectos::TypeInfo* getDerivedTypes();

#endif
