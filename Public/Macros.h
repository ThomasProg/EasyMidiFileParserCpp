#ifndef _MACROS_H_
#define _MACROS_H_

#ifndef MAKE_DLL
#define MAKE_DLL
#endif

#ifdef MAKE_DLL // CPP
// #define MIDIPARSEREXPORT __declspec(dllexport)
// #else
// #define MIDIPARSEREXPORT __declspec(dllimport)
// #endif


#if defined(_WIN32)
    #define MIDIPARSEREXPORT __declspec(dllexport)
#elif defined(__GNUC__)
    #define MIDIPARSEREXPORT __attribute__((visibility("default")))
#else
    #define MIDIPARSEREXPORT
#endif

#endif

#endif 