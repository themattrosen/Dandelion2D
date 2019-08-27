#pragma once

// OS defines
#ifdef _WIN32
#	define DL_WINDOWS
#else
#	error Dandelion2D only supports Windows!
#endif

// DLL defines
#ifdef DL_WINDOWS
#	ifdef DL_BUILD_DLL
#		define DANDELION_API __declspec(dllexport)
#	else
#		define DANDELION_API __declspec(dllimport)
#	endif
#endif

// assertions
#include <cassert>
//#ifdef _DEBUG
#	define ASSERT(cond) \
		if((cond)) {} else { __debugbreak(); }
//#else
//#	define ASSERT(cond) 
//#endif
#define STATIC_ASSERT(cond, ...) static_assert(cond, __VA_ARGS__) 

// alignment
#define DL_ALIGN(num_bytes) __declspec(align( (num_bytes) ))

// memory usage
#define MAX_GAME_OBJECTS 50
#define MAX_GAME_OBJECT_ARCHETYPES 30
#define MAX_CONCURRENT_AUDIO_VOICES 30
#define MAX_AUDIO_ASSETS 30

// type defs
using f32 = float;
using f64 = double;
using s8 = signed char;
using u8 = unsigned char;
using s16 = signed short;
using u16 = unsigned short;
using s32 = signed int;
using u32 = unsigned int;
using s64 = __int64;
using string = const char*;
