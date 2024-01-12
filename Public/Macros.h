#ifndef _MACROS_H_
#define _MACROS_H_

#ifndef MAKE_DLL
#define MAKE_DLL
#endif

#ifdef MAKE_DLL // CPP
#define MIDIPARSEREXPORT __declspec(dllexport)
#else
#define MIDIPARSEREXPORT __declspec(dllimport)
#endif

#endif 