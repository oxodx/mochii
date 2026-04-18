#pragma once
#include <memory>

#ifdef _WIN32
#ifdef _WIN64
#define MI_PLATFORM_WINDOWS
#else
#error "x86 Builds are not supported!"
#endif
#elif defined(__APPLE__) || defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#error "IOS simulator is not supported!"
#elif TARGET_OS_IPHONE == 1
#define MI_PLATFORM_IOS
#error "IOS is not supported!"
#elif TARGET_OS_MAC == 1
#define MI_PLATFORM_MACOS
#error "MacOS is not supported!"
#else
#error "Unknown Apple platform!"
#endif
#elif defined(__ANDROID__)
#define MI_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define MI_PLATFORM_LINUX
#error "Linux is not supported!"
#else
#error "Unknown platform!"
#endif

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
#error Mochii only supports Windows!
#endif

#ifdef MI_DEBUG
#define MI_ENABLE_ASSERTS
#endif

#ifdef MI_ENABLE_ASSERTS
#define MI_ASSERT(x, ...)                             \
  {                                                   \
    if (!(x)) {                                       \
      MI_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
      __debugbreak();                                 \
    }                                                 \
  }
#define MI_CORE_ASSERT(x, ...)                             \
  {                                                        \
    if (!(x)) {                                            \
      MI_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
      __debugbreak();                                      \
    }                                                      \
  }
#else
#define MI_ASSERT(x, ...)
#define MI_CORE_ASSERT(x, ...)
#endif

#define BIT(x) ((1u) << (x))

#define MI_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Mochii {
template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
Scope<T> CreateScope(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
Ref<T> CreateRef(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}
}  // namespace Mochii
