#pragma once
#include "Mochii/Core/Base.h"
#include "Mochii/Core/Log.h"

#ifdef MI_ENABLE_ASSERTS
namespace Mochii::Assert {
constexpr const char* CurrentFileName(const char* path) {
  const char* file = path;
  while (*path) {
    if (*path == '/' || *path == '\\')
      file = ++path;
    else
      path++;
  }
  return file;
}
}

#define MI_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
  {                                                    \
    if (!(check)) {                                    \
      MI##type##ERROR(msg, __VA_ARGS__);               \
      MI_DEBUGBREAK();                                 \
    }                                                  \
  }
#define MI_INTERNAL_ASSERT_WITH_MSG(type, check, ...) \
  MI_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define MI_INTERNAL_ASSERT_NO_MSG(type, check)                              \
  MI_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", \
                          MI_STRINGIFY_MACRO(check),                        \
                          ::Mochii::Assert::CurrentFileName(__FILE__),       \
                          __LINE__)

#define MI_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define MI_INTERNAL_ASSERT_GET_MACRO(...)            \
  MI_EXPAND_MACRO(MI_INTERNAL_ASSERT_GET_MACRO_NAME( \
      __VA_ARGS__, MI_INTERNAL_ASSERT_WITH_MSG, MI_INTERNAL_ASSERT_NO_MSG))

#define MI_ASSERT(...) \
  MI_EXPAND_MACRO(MI_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define MI_CORE_ASSERT(...) \
  MI_EXPAND_MACRO(          \
      MI_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
#define MI_ASSERT(...)
#define MI_CORE_ASSERT(...)
#endif
