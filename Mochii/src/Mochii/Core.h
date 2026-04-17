#pragma once

#ifdef MI_PLATFORM_WINDOWS
	#if MI_DYNAMIC_LINK
		#ifdef MI_BUILD_DLL
			#define MOCHII_API __declspec(dllexport)
		#else
			#define MOCHII_API __declspec(dllimport)
		#endif
	#else
		#define MOCHII_API
	#endif
#else
	#error "Mochii only supports Windows!"
#endif

#ifdef MI_DEBUG
	#define MI_ENABLE_ASSERTS
#endif

#ifdef MI_ENABLE_ASSERTS
	#define MI_ASSERT(x, ...) { if(!(x)) { MI_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MI_CORE_ASSERT(x, ...) { if(!(x)) { MI_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MI_ASSERT(x, ...)
	#define MI_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define MI_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
