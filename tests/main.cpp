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
#include "baselib/baseclass.h"
#include "derivedlib/derivedclass.h"

#include <cstdio>
#ifdef UNITOS_WINDOWS
#   include <windows.h>
#   ifndef NDEBUG
#       include <crtdbg.h>
#   endif
#endif

class ErrorOutput : public unitos::Output
{
public:
    ErrorOutput() {}
    virtual ~ErrorOutput() {}
    virtual void operator<<(unitos::String const& text) {
        fprintf(stderr, "%s", text.c_str());
    }
};

#if 0
void loadSharedTypes(char const* filename)
{
    HMODULE lib = ::LoadLibraryA(filename);
    if(lib == NULL)
        return;
    
    typedef reflectos::TypeInfo* (*getFunc)();
    getFunc func = (getFunc)::GetProcAddress(lib, "getSharedTypes");
    if(func == nullptr)
        return;

    reflectos::TypeInfo* sharedTypes = func();
    reflectos::Registry::merge(sharedTypes);
}
#endif

int main(int argc, char *argv[])
{
    argc, argv;
    
    //loadSharedTypes("reflectos_unittest_sharedlib");
    reflectos::Registry::merge(getBaseTypes());
    reflectos::Registry::merge(getDerivedTypes());
    reflectos::Registry::init();

#if defined(UNITOS_WINDOWS) && !(NDEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
    //_CrtSetBreakAlloc(218);
#endif

    REGISTER_OUTPUT(ErrorOutput);
    RUN_ALL_TESTS();
    return 0;
}
