#pragma once
#include <filesystem>
#include "Mochii/Core/Base.h"
#include "Mochii/Core/Log.h"

#ifdef MI_ENABLE_ASSERTS
// Alternatively we could use the same "default" message for both "WITH_MSG" and
// "NO_MSG" and provide support for custom formatting by concatenating the
// formatting string instead of having the format inside the default message
#define MI_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
  {                                                    \
    if (!(check)) {                                    \
      MI##type##ERROR(msg, __VA_ARGS__);               \
      MI_DEBUGBREAK();                                 \
    }                                                  \
  }
#define MI_INTERNAL_ASSERT_WITH_MSG(type, check, ...) \
  MI_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define MI_INTERNAL_ASSERT_NO_MSG(type, check)                                 \
  MI_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}",    \
                          MI_STRINGIFY_MACRO(check),                           \
                          std::filesystem::path(__FILE__).filename().string(), \
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
