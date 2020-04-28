#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <thread>
#include <mutex>

#include <type_traits>
#include <typeinfo>
#include <climits>

#include <chrono>
#include <locale>

#include <memory>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>

#include <functional>
#include <utility>
#include <algorithm>

#ifdef PLATFORM_LINUX
	#ifndef NDEBUG
#include <signal.h>
	#endif // NDEBUG
#endif // PLATFORM_LINUX

#ifdef PLATFORM_WINDOWS
// Slim Windows

// target Windows 8 or later
#define _WIN32_WINNT _WIN32_WINNT_WIN8
#include <sdkddkver.h>

#define WIN32_LEAN_AND_MEAN

#define WIN32_EXTRA_LEAN

#define NOIME
#define NOWINRES
#define NOGDICAPMASKS
#define NOVIRTUALKEYCODES
#define NOWINMESSAGES
#define NOWINSTYLES
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOKEYSTATES
#define NOSYSCOMMANDS
#define NORASTEROPS
#define NOSHOWWINDOW
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOGDI // MAKEPOINTS
#define NOUSER
#define NOMB
#define NOMEMMGR
#define NOMETAFILE
#define NOMINMAX
#define NOMSG
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOWINOFFSETS // SetWindowLongPtrA
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NOIME
#define NOPROXYSTUB
#define NOIMAGE
#define NO
#define NOTAPE
#define ANSI_ONLY

#define STRICT

#include <Windows.h>

#endif // PLATFORM_WINDOWS