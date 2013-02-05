// Copyright (C) Ville Ruusutie, 2010

#include "unitos/unitos.h"
#include "reflectos.h"
#include <cstdio>
#if defined(__UNITOS_WINDOWS__) && !(NDEBUG)
#include <crtdbg.h>
#endif

class ErrorOutput : public unitos::Output
{
public:
    ErrorOutput() {}
    virtual ~ErrorOutput() {}
    virtual void operator<<(unitos::String const& text) {
        fprintf(stderr, "%s", text.GetCStr());
    }
};

int main(int argc, char *argv[])
{
    reflectos::Registry::init();

#if defined(__UNITOS_WINDOWS__) && !(NDEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

    REGISTER_OUTPUT(ErrorOutput);
    RUN_ALL_TESTS();
    return 0;
}
