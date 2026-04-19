#pragma once
#include <memory>
#include "Mochii/Core/PlatformDetection.h"

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

#define MI_EXPAND_MACRO(x) x
#define MI_STRINGIFY_MACRO(x) #x

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

#include "Mochii/Core/Assert.h"
#include "Mochii/Core/Log.h"

