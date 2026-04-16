#pragma once

#ifdef MI_PLATFORM_WINDOWS
	#ifdef MI_BUILD_DLL
		#define MOCHII_API __declspec(dllexport)
	#else
		#define MOCHII_API __declspec(dllimport)
	#endif
#else
	#error "Mochii only supports Windows!"
#endif

#define BIT(x) (1 << x)
