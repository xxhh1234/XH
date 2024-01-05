#pragma once

#ifdef XH_PLATFORM_WINDOWS
	#ifdef XH_BUILD_DLL
		#define XH_API _declspec(dllexport)
	#else
		#define XH_API _declspec(dllimport)
	#endif
#else 
	#error XH only supports Windows!
#endif

#define BIT(x) (1<<x)

#ifdef XH_ENABLE_ASSERTS
	#define XH_ASSERT(x, ...) {if(!(x)) { XH_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define XH_CORE_ASSERT(x, ...) {if(!(x)) { XH_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define XH_ASSERT(x, ...)
	#define XH_CORE_ASSERT(x, ...)
#endif
		
