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
