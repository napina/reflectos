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
#ifndef BASE_CLASS_H
#define BASE_CLASS_H
#include "reflectos.h"

#ifdef BUILD_BASE_DLL
#define BASE_DLL __declspec(dllexport)
#else
#define BASE_DLL __declspec(dllimport)
#endif

class SharedBaseClass
{
public:
    BASE_DLL SharedBaseClass();
    BASE_DLL virtual ~SharedBaseClass();

    BASE_DLL int getData();
    BASE_DLL virtual int test();

protected:
    int m_data;

    REFLECT_BASE_CLASS(SharedBaseClass)
        REFLECT_FIELD(m_data)
        REFLECT_FUNCTION(getData)
        REFLECT_FUNCTION(test)
    REFLECT_END()
};

BASE_DLL reflectos::TypeInfo* getBaseTypes();

#endif
