//
//  Platform.h
//  Project Unknown
//
//  Created by Robert Bu on 11/21/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Platform_h
#define Project_Unknown_Platform_h

#if !defined(__cplusplus)
    #error C++ compiler required.
#endif

#if defined(DEBUG) || defined(_DEBUG)
    #define UKN_DEBUG
#endif


#if defined(__GNUC__)

    #define UKN_COMPILER_GCC

    #if __GNUC_MINOR__ >= 5
        #define UKN_COMPILER_VERSION 45
    #elif __GNUC_MINOR__ >= 4
        #define UKN_COMPILER_VERSION 44
    #elif __GNUC_MINOR__ >= 3
        #define UKN_COMPILER_VERSION 43
    #elif __GNUC_MINOR__ >= 2
        #define UKN_COMPILER_VERSION 42
    #elif __GNUC_MINOR__ >= 1
        #define UKN_COMPILER_VERSION 41
    #elif __GNUC_MINOR__ >= 0
        #define UKN_COMPILER_VERSION 40
    #else
        #error Unknown Compiler
    #endif

    #if __GNUC_MINOR__ >= 3
        #ifdef __GXX_EXPERIMENTAL_CXX0X__
            #define UKN_CXX0X_SUPPORT
        #endif // __GXX_EXPERIMENTAL_CXX0X__
    #endif // __GNUC_MINOR__

#elif defined(_MSC_VER)

    #define UKN_COMPILER_MSVC
    #define UKN_HAS_DECLSPEC

    #if _MSC_VER >= 1600
        #define UKN_COMPILER_VERSION 100
        #define UKN_CXX0X_SUPPORT
    #elif _MSC_VER >= 1500
        #define UKN_COMPILER_VERSION 90
    #elif _MSC_VER >= 1400
        #define UKN_COMPILER_VERSION 80
    #else
        #error Unknown Compiler
    #endif

#endif

// vc++ override specification
#if !defined(UKN_COMPILER_MSVC)
#define override
#endif


// apple os ( osx / ios )
#if defined(__APPLE__) || defined(__APPLE_CC__)

#include <Availability.h>

    #if !defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
        #define UKN_OS_OSX
    #else
        #define UKN_OS_IOS
    #endif

    #if MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_7
        #define UKN_APPLE_OSX_LION
    #endif
    
    #define UKN_OS_FAMILY_APPLE
    #define UKN_OS_FAMILY_UNIX

// android
#elif defined(__ANDROID__)
    #define UKN_OS_ANDROID
    #define UKN_OS_FAMILY_UNIX

// windows
#elif defined(_WIN32) || defined(_MSC_VER)
    #define UKN_OS_WINDOWS

    #if defined(_WIN64)
        #define UKN_PLATFORM_64
    #else
        #define UKN_PLATFORM_32
    #endif

    #ifndef NOMINMAX
        #define NOMINMAX
    #endif

    #if defined(__MINGW32__)
        #include <_mingw.h>
    #endif

    #if defined(__CYGWIN__)
        #define UKN_PLATFORM_CYGWIN
        #define UKN_HAS_DECLSPEC
    #endif

// linux
#elif defined(linux) || defined(__linux)
    #define UKN_OS_LINUX
    #define UKN_OS_FAMILY_UNIX

// psp
#elif defined(_PSP)
    #define UKN_OS_PSP

#else
    #error Unknown Platform
#endif // UKN_PLATFORM

#if defined(UKN_COMPILER_MSVC)
    #if defined(_M_X64) 
        #define UKN_CPU_X64
        #define UKN_COMPILER_TARGET x64
    #elif defined(_M_IX86)
        #define UKN_CPU_X86
        #define UKN_COMPILER_TARGET x86
    #endif
    
#elif defined(UKN_COMPILER_GCC)
    #if defined(__x86_64__)
        #define UKN_CPU_X64
        #define UKN_COMPILER_TARGET x64
    #elif defined(__i386__)
        #define UKN_CPU_X86
        #define UKN_COMPILER_TARGET x86
    #endif
#endif

#if defined(UKN_CPU_PPC)
    #define UKN_BIG_ENDIAN

#elif defined(UKN_CPU_X86) || defined(UKN_CPU_X64) || defined(UKN_OS_WINDOWS)
    #define UKN_LITTLE_ENDIAN

#else
    #define UKN_BIG_ENDIAN
#endif

#ifdef UKN_CPU_X64
    #define _SSE_SUPPORT
    #define _SSE2_SUPPORT
    #define _X64_SUPPORT

#elif defined UKN_CPU_X86
    #if defined(UKN_COMPILER_MSVC)
        #if _M_IX86 == 600
            #define _MMX_SUPPORT
        #endif

        #if _M_IX86_FP == 1
            #define _SSE_SUPPORT
        #elif _M_IX86_FP == 2
            #define _SSE_SUPPORT
            #define _SSE2_SUPPORT
        #endif

    #elif defined(UKN_COMPILER_GCC)
        #ifdef __MMX__
            #define _MMX_SUPPORT
        #endif
        #ifdef __SSE__
            #define _SSE_SUPPORT
        #endif
        #ifdef __SSE2__
            #define _SSE2_SUPPORT
        #endif
    #endif // UKN_COMPILER_GCC
#endif // UKN_CPU_X64

#include <cassert>

#if defined(UKN_STD_CALL)
    #define UKN_CALL __stdcall
#else
    #define UKN_CALL
#endif

#if defined(UKN_OS_WINDOWS)
    #define strcmpnocase stricmp
#else
    #define strcmpnocase strcasecmp
#endif

// are we building a dll or not
#if defined(UKN_OS_WINDOWS) && defined(UKN_HAS_DECLSPEC)
    #if defined(UKN_DLL_EXPORT)
        #define UKN_API __declspec(dllexport)
        #define UKN_EXTERN extern
    #elif defined(UKN_DLL_IMPORT)
        #define UKN_API __declspec(dllimport)
        #define UKN_EXTERN extern
    #else
        #define UKN_API
        #define UKN_EXTERN
    #endif
#else
    #define UKN_API
    #define UKN_EXTERN
#endif

#define ukn_assert assert

#ifdef UKN_OS_WINDOWS
#define __inline__ inline
#endif

#ifndef __GNUC__
    #define snprintf _snprintf
#endif

// #define UKN_ENABLE_MULTI_THREAD

#include "Type.h"

#endif // Project_Unknown_Platform_h
