//
//  Config.h
//  Project Unknown
//
//  Created by Robert Bu on 11/21/11.
//  Copyright (c) 2011 heizi. All rights reserved.
//

#ifndef Project_Unknown_Type_h
#define Project_Unknown_Type_h

#if defined(_WIN32)
    #if !defined(_WIN32_WINNT)
        #define _WIN32_WINNT 0x0500
#endif
    #if !defined(WIN32_LEAN_AND_MEAN)
        #define WIN32_LEAN_AND_MEAN
    #endif
#endif

// Turn off some annoying warnings
#if defined(_MSC_VER)
    #pragma warning(disable:4018) // signed/unsigned comparison
    #pragma warning(disable:4251) // ... needs to have dll-interface warning
    #pragma warning(disable:4355) // 'this' : used in base member initializer list
    #pragma warning(disable:4996) // VC++ 8.0 deprecation warnings
    #pragma warning(disable:4351) // new behavior: elements of array '...' will be default initialized
    #pragma warning(disable:4675) // resolved overload was found by argument-dependent lookup

    #pragma warning(disable:4786)  // identifier truncation warning
    #pragma warning(disable:4503)  // decorated name length exceeded - mainly a problem with STLPort
    #pragma warning(disable:4284)  // return type for operator -> is not UDT
#endif

#if defined(__intEL_COMPILER)
    #pragma warning(disable:1738) // base class dllexport/dllimport specification differs from that of the derived class
    #pragma warning(disable:1478) // function ... was declared "deprecated"
    #pragma warning(disable:1744) // field of class type without a DLL interface used in a class with a DLL interface
#endif

#include <stdint.h>
#include <string>

namespace ukn {

#if defined(_MSC_VER)
    //
    // Windows/Visual C++
    //
    typedef signed char            int8;
    typedef unsigned char          uint8;
    typedef signed short           int16;
    typedef unsigned short         uint16;
    typedef signed int             int32;
    typedef unsigned int           uint32;
    typedef signed __int64         int64;
    typedef unsigned __int64       uint64;
#if defined(_WIN64)
#define _PTR_IS_64_BIT 1
    typedef signed __int64     intPtr;
    typedef unsigned __int64   uintPtr;
#else
    typedef signed long        intPtr;
    typedef unsigned long      uintPtr;
#endif
#ifndef _HAVE_int64
#define _HAVE_int64 1
#endif
#elif defined(__GNUC__)
    //
    // Unix/GCC
    //
    typedef signed char            int8;
    typedef unsigned char          uint8;
    typedef signed short           int16;
    typedef unsigned short         uint16;
    typedef signed int             int32;
    typedef unsigned int           uint32;
    typedef signed long            intPtr;
    typedef unsigned long          uintPtr;
#if defined(__LP64__)
#define _PTR_IS_64_BIT 1
#define _LONG_IS_64_BIT 1
    
#ifdef _UINT64
#undef _UINT64
#endif
    
#ifdef __INT64
#undef __INT64
#endif
    typedef int64_t        int64;
    typedef uint64_t		uint64;
#else
    typedef signed long long   int64;
    typedef unsigned long long uint64;
#endif
#ifndef _HAVE_int64
#define _HAVE_int64 1
#endif
#elif defined(__DECCXX)
    //
    // Compaq C++
    //
    typedef signed char            int8;
    typedef unsigned char          uint8;
    typedef signed short           int16;
    typedef unsigned short         uint16;
    typedef signed int             int32;
    typedef unsigned int           uint32;
    typedef signed __int64         int64;
    typedef unsigned __int64       uint64;
#if defined(__VMS)
#if defined(__32BITS)
    typedef signed long    intPtr;
    typedef unsigned long  uintPtr;
#else
    typedef int64          intPtr;
    typedef uint64         uintPtr;
#define _PTR_IS_64_BIT 1
#endif
#else
    typedef signed long        intPtr;
    typedef unsigned long      uintPtr;
#define _PTR_IS_64_BIT 1
#define _LONG_IS_64_BIT 1
#endif
#define _HAVE_int64 1
#elif defined(__HP_aCC)
    //
    // HP Ansi C++
    //
    typedef signed char            int8;
    typedef unsigned char          uint8;
    typedef signed short           int16;
    typedef unsigned short         uint16;
    typedef signed int             int32;
    typedef unsigned int           uint32;
    typedef signed long            intPtr;
    typedef unsigned long          uintPtr;
#if defined(__LP64__)
#define _PTR_IS_64_BIT 1
#define _LONG_IS_64_BIT 1
    typedef signed long        int64;
    typedef unsigned long      uint64;
#else
    typedef signed long long   int64;
    typedef unsigned long long uint64;
#endif
#define _HAVE_int64 1
#elif defined(__SUNPRO_CC)
    //
    // SUN Forte C++
    //
    typedef signed char            int8;
    typedef unsigned char          uint8;
    typedef signed short           int16;
    typedef unsigned short         uint16;
    typedef signed int             int32;
    typedef unsigned int           uint32;
    typedef signed long            intPtr;
    typedef unsigned long          uintPtr;
#if defined(__sparcv9)
#define _PTR_IS_64_BIT 1
#define _LONG_IS_64_BIT 1
    typedef signed long        int64;
    typedef unsigned long      uint64;
#else
    typedef signed long long   int64;
    typedef unsigned long long uint64;
#endif
#define _HAVE_int64 1
#elif defined(__IBMCPP__)
    //
    // IBM XL C++
    //
    typedef signed char            int8;
    typedef unsigned char          uint8;
    typedef signed short           int16;
    typedef unsigned short         uint16;
    typedef signed int             int32;
    typedef unsigned int           uint32;
    typedef signed long            intPtr;
    typedef unsigned long          uintPtr;
#if defined(__64BIT__)
#define _PTR_IS_64_BIT 1
#define _LONG_IS_64_BIT 1
    typedef signed long        int64;
    typedef unsigned long      uint64;
#else
    typedef signed long long   int64;
    typedef unsigned long long uint64;
#endif
#define _HAVE_int64 1
#elif defined(__sgi)
    //
    // MIPSpro C++
    //
    typedef signed char            int8;
    typedef unsigned char          uint8;
    typedef signed short           int16;
    typedef unsigned short         uint16;
    typedef signed int             int32;
    typedef unsigned int           uint32;
    typedef signed long            intPtr;
    typedef unsigned long          uintPtr;
#if _MIPS_SZLONG == 64
#define _PTR_IS_64_BIT 1
#define _LONG_IS_64_BIT 1
    typedef signed long        int64;
    typedef unsigned long      uint64;
#else
    typedef signed long long   int64;
    typedef unsigned long long uint64;
#endif
#define _HAVE_int64 1
#endif
    
    typedef std::string ukn_string;
    typedef std::wstring ukn_wstring;
    
#define UKN_SINGLE_PRECISION
    
#ifdef UKN_SINGLE_PRECISION
    typedef		float			real;
#else
    typedef     double          real;
#endif
    
} // namespace ukn


#if defined(UKN_CXX0X_SUPPORT)
    #ifdef UKN_OS_WINDOWS
        #include <unordered_map>
    #endif
    #define ukn_hash_map std::unordered_map    
#else 
    #define ukn_hash_map std::map
#endif // UKN_CXX0X_SUPPORT

#endif // Project_Unknown_Config_h
