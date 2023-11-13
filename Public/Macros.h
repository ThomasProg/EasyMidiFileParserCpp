#ifndef _MACROS_H_
#define _MACROS_H_

#define MAKE_DLL

#ifdef MAKE_DLL // CPP
#define MIDIPARSEREXPORT __declspec(dllexport)
#else
#define MIDIPARSEREXPORT __declspec(dllimport)
#endif

#endif 