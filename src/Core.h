#pragma once
#include "Utility/UtilityMacros.h"

static constexpr const char* glsl_version = "#version 410";

#if defined(PLATFORM_WINDOWS)
	#define MAIN() CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
	#define PRAGMA(x) __pragma(x)

	#ifndef NDEBUG
		#define	DEBUG_BREAK() __debugbreak()
		#define DebugLog(x) std::ostringstream APPLINE(oss); APPLINE(oss) << x; OutputDebugString(APPLINE(oss).str().c_str())
	#else
		#define DebugLog(x)
	#endif
#elif defined(PLATFORM_LINUX)
	#define MAIN() main()
	#define PRAGMA(x)

	#ifndef NDEBUG
		#define	DEBUG_BREAK() raise(SIGTRAP);
		#define DebugLog(x) std::clog << x
	#else
		#define DebugLog(x)
	#endif
#else
	#error Only Windows and Linux Supported for now!
#endif


#ifndef NDEBUG
	#define NIC_ASSERT(x,s) if(!(x)){ DebugLog("ASSERTION FALIED: " << s << "\nLINE: " << __LINE__ << "\nFILE: " << __FILE__ << '\n'); DEBUG_BREAK();}
#else
	#define NIC_ASSERT(x,s) PRAGMA(warning(disable: 552 553 834)) x; PRAGMA(warning(default: 552 553 834))
#endif