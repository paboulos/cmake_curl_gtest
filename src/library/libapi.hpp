#pragma once

#if defined(_WIN64) || defined(_WIN32)
#  ifdef _DLL
#  define EXPORT __declspec(dllexport)
#  else
#  define EXPORT __declspec(dllimport)
#endif
#else
#  define EXPORT
#  define WINAPI
#endif

#include "stdafx.hpp"

// The abstract interface for Xyz object.
// No extra specifiers required.
typedef enum { ALPHA=0, BETA, GAMMA, NIL} greek_t;
typedef const char* letters_t; 
struct IAbc
{
    virtual letters_t GreekIndex(greek_t greek) = 0;
    virtual void Release() = 0;
    virtual letters_t GreekName(greek_t greek) = 0;
};


typedef IAbc* AbcHANDLE;
#define EXTERN_C  extern "C" 

// Factory function that creates instances of the Xyz object.
EXTERN_C EXPORT AbcHANDLE WINAPI CreateAbc();