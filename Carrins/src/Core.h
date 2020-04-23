#pragma once
#include "UtilityMacros.h"

const char* glsl_version = "#version 410";

#if defined(PLATFORM_WINDOWS)
	#define MAIN() CALLBACK WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
	#define	DEBUG_BREAK() __debugbreak()

	#ifndef NDEBUG
		#define DebugLog(x) std::ostringstream APPLINE(oss); APPLINE(oss) << x; OutputDebugString(APPLINE(oss).str().c_str())
	#else
		#define DebugLog(x)
	#endif
#elif defined(PLATFORM_LINUX)
	#define MAIN() main()
	#define	DEBUG_BREAK() raise(SIGTRAP);

	#ifndef NDEBUG
		#define DebugLog(x) std::cout << x
	#else
		#define DebugLog(x)
	#endif
#else
	#error Only Windows and Linux Supported for now!
#endif

#ifndef NDEBUG
	#define ASSERT(x,s) if(!(x)){ DebugLog("ASSERTION FALIED: " << s << "\nLINE: " << __LINE__ << "\nFILE: " << __FILE__ << '\n'); DEBUG_BREAK();}
#else
	#define ASSERT(x,s)
#endif