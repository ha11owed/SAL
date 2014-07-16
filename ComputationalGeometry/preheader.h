#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif
#define NOMINMAX // exclude microsoft definition of min/max

// allow deprecated printf
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#ifdef _MSC_VER
// DUMP MEMORY LEAKS
#include <crtdbg.h>
#endif

#include <windows.h>
#include <stdio.h>
#include <shellapi.h>

