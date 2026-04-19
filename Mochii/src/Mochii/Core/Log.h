#pragma once
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include "Mochii/Core/Base.h"

namespace Mochii {
class Log {
 public:
  static void Init();

  static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
  static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

 private:
  static Ref<spdlog::logger> s_CoreLogger;
  static Ref<spdlog::logger> s_ClientLogger;
};
}  // namespace Mochii

// Core log macros
#define MI_CORE_TRACE(...) ::Mochii::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MI_CORE_INFO(...) ::Mochii::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MI_CORE_WARN(...) ::Mochii::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MI_CORE_ERROR(...) ::Mochii::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MI_CORE_CRITICAL(...) \
  ::Mochii::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define MI_TRACE(...) ::Mochii::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MI_INFO(...) ::Mochii::Log::GetClientLogger()->info(__VA_ARGS__)
#define MI_WARN(...) ::Mochii::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MI_ERROR(...) ::Mochii::Log::GetClientLogger()->error(__VA_ARGS__)
#define MI_CRITICAL(...) ::Mochii::Log::GetClientLogger()->critical(__VA_ARGS__)
