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

#ifdef MI_DEBUG
#if defined(MI_PLATFORM_WINDOWS)
#define MI_DEBUGBREAK() __debugbreak()
#elif defined(MI_PLATFORM_LINUX)
#include <signal.h>
#define MI_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define MI_ENABLE_ASSERTS
#else
#define MI_DEBUGBREAK()
#endif

#ifdef MI_ENABLE_ASSERTS
#define MI_ASSERT(x, ...)                             \
  {                                                   \
    if (!(x)) {                                       \
      MI_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
      MI_DEBUGBREAK();                                \
    }                                                 \
  }
#define MI_CORE_ASSERT(x, ...)                             \
  {                                                        \
    if (!(x)) {                                            \
      MI_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
      MI_DEBUGBREAK();                                     \
    }                                                      \
  }
#else
#define MI_ASSERT(x, ...)
#define MI_CORE_ASSERT(x, ...)
#endif

#define BIT(x) ((1u) << (x))

#define MI_BIND_EVENT_FN(fn)                                \
  [this](auto&&... args) -> decltype(auto) {                \
    return this->fn(std::forward<decltype(args)>(args)...); \
  }

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
