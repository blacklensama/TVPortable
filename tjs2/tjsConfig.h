//---------------------------------------------------------------------------
/*
	TJS2 Script Engine
	Copyright (C) 2000 W.Dee <dee@kikyou.info> and contributors

	See details of license at "license.txt"
*/
//---------------------------------------------------------------------------
// configuration
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#ifndef tjsConfigH
#define tjsConfigH

namespace TJS
{
//---------------------------------------------------------------------------


/*
	many settings can be changed here.

	tjsCommHead.h includes most common headers that will be needed to
	compile the entire TJS program.

	configuration about Critical Section for multithreading support is there in
	tjsUtils.cpp/h.
*/

// TODO: autoconf integration

#include "tjsTypes.h"
#include "stdarg.h"


// #define TJS_NO_AVOID_ISWDIGIT
// #define TJS_SUPPORT_VCL
// #define TJS_MSG_EMPTY
// #define TJS_DEBUG_TRACE
// #define TJS_JP_LOCALIZED
// #define TJS_TEXT_OUT_CRLF
// #define TJS_WITH_IS_NOT_RESERVED_WORD

TJS_EXP_FUNC_DEF(tjs_int, TJS_atoi, (const tjs_char *s));
TJS_EXP_FUNC_DEF(tjs_char *, TJS_int_to_str, (tjs_int value, tjs_char *string));
TJS_EXP_FUNC_DEF(tjs_char *, TJS_tTVInt_to_str, (tjs_int64 value, tjs_char *string));
TJS_EXP_FUNC_DEF(tjs_int, TJS_strnicmp, (const tjs_char *s1, const tjs_char *s2, size_t maxlen));
TJS_EXP_FUNC_DEF(tjs_int, TJS_stricmp, (const tjs_char *s1, const tjs_char *s2));
TJS_EXP_FUNC_DEF(void, TJS_strcpy_maxlen, (tjs_char *d, const tjs_char *s, size_t len));
TJS_EXP_FUNC_DEF(void, TJS_strcpy, (tjs_char *d, const tjs_char *s));
TJS_EXP_FUNC_DEF(size_t, TJS_strlen, (const tjs_char *d));

#define TJS_strcmp			wcscmp
#define TJS_strncmp			wcsncmp
#define TJS_strncpy			wcsncpy
#define TJS_strcat			wcscat
#define TJS_strstr			wcsstr
#define TJS_strchr			wcschr
#define TJS_malloc			malloc
#define TJS_free			free
#define TJS_realloc			realloc
#define TJS_nsprintf		sprintf
#define TJS_nstrcpy			strcpy
#define TJS_nstrcat			strcat
#define TJS_nstrlen			strlen
#define TJS_strftime		wcsftime
#define TJS_vfprintf		vfwprintf
#define TJS_octetcpy		memcpy
#define TJS_octetcmp		memcmp
#define TJS_strtod			wcstod


#if defined(__GNUC__)
	#define TJS_cdecl
	#define TJS_vsnprintf		vswprintf
	extern tjs_int TJS_sprintf(tjs_char *s, const tjs_char *format, ...);
	#define TJS_timezone timezone
	#define TJS_rand rand
	#define TJS_RAND_MAX RAND_MAX
	#define TJS_mbstowcs mbstowcs
	#define TJS_wcstombs wcstombs
	#define TJS_mbtowc   mbtowc
	#define TJS_wctomb   wctomb
#elif defined(_WIN32) || defined(_MSC_VER)

#include <stdio.h>
#include <wchar.h>

#define M_PI            3.14159265358979323846264338327950288
#define M_2PI           6.28318530717958647692528676655900576
#define M_3PI_4         2.35619449019234492884698253745962716
#define M_PI_2          1.57079632679489661923132169163975144
#define M_3PI_8         1.17809724509617246442349126872981358
#define M_PI_4          0.78539816339744830961566084581987572
#define M_PI_8          0.39269908169872415480783042290993786
#define M_1_PI          0.31830988618379067153776752674502872
#define M_2_PI          0.63661977236758134307553505349005744
#define M_4_PI          1.27323954473516268615107010698011488
#define M_E             2.71828182845904523536028747135266250
#define M_LOG2E         1.44269504088896340735992468100189213
#define M_LOG10E        0.43429448190325182765112891891660508
#define M_LN2           0.69314718055994530941723212145817657
#define M_LN10          2.30258509299404568401799145468436421
#define M_SQRT2         1.41421356237309504880168872420969808
#define M_1_SQRT2       0.70710678118654752440084436210484904
#define M_EULER         0.57721566490153286060651209008240243

	#define TJS_cdecl __cdecl
	#define TJS_vsnprintf		_vsnwprintf
	#define TJS_sprintf			swprintf
	#define TJS_timezone _timezone
	#define TJS_rand rand
	#define TJS_RAND_MAX RAND_MAX
	extern size_t TJS_mbstowcs(tjs_char *pwcs, const tjs_nchar *s, size_t n);
	extern size_t TJS_wcstombs(tjs_nchar *s, const tjs_char *pwcs, size_t n);
	extern int TJS_mbtowc(tjs_char *pwc, const tjs_nchar *s, size_t n);
	extern int TJS_wctomb(tjs_nchar *s, tjs_char wc);

	#pragma warning(disable:4800) // force bool
	#pragma warning(disable:4099) // class & struct
	#pragma warning(disable:4996) // _s c functions instead
#endif


#define TJS_narrowtowidelen(X) TJS_mbstowcs(NULL, (X),0) // narrow->wide (if) converted length
#define TJS_narrowtowide TJS_mbstowcs

void TJS_debug_out(const tjs_char *format, ...);

#ifdef TJS_DEBUG_TRACE
#define TJS_D(x)	TJS_debug_out x;
#define TJS_F_TRACE(x) tTJSFuncTrace ___trace(TJS_W(x));
#else
#define TJS_D(x)
#define TJS_F_TRACE(x)
#endif

extern void TJSNativeDebuggerBreak();


extern void TJSSetFPUE();
extern void TJSRestoreFPUE();


//---------------------------------------------------------------------------
// elapsed time profiler
//---------------------------------------------------------------------------
#ifdef TJS_DEBUG_PROFILE_TIME
extern tjs_uint TJSGetTickCount();
class tTJSTimeProfiler
{
	tjs_uint & timevar;
	tjs_uint start;
public:
	tTJSTimeProfiler(tjs_uint & tv) : timevar(tv)
	{
		start = TJSGetTickCount();
	}

	~tTJSTimeProfiler()
	{
		timevar += TJSGetTickCount() - start;
	}
};
#endif
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// function tracer
//---------------------------------------------------------------------------
class tTJSFuncTrace
{
	tjs_char *funcname;
public:
	tTJSFuncTrace(tjs_char *p)
	{
		funcname = p;
		TJS_debug_out(TJS_W("enter: %ls\n"), funcname);
	}
	~tTJSFuncTrace()
	{
		TJS_debug_out(TJS_W("exit: %ls\n"), funcname);
	}
};
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// tTJSNarrowStringHolder : converts wide -> narrow, and holds it until be destroyed
//---------------------------------------------------------------------------
struct tTJSNarrowStringHolder
{
	bool Allocated;
	tjs_nchar *Buf;
public:
	tTJSNarrowStringHolder(const wchar_t *wide);

	~tTJSNarrowStringHolder(void);

	operator const tjs_nchar *()
	{
		return Buf;
	}
};
//---------------------------------------------------------------------------

}


#endif

