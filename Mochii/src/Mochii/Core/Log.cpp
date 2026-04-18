#include "Mochii/Core/Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include "mzpch.h"

namespace Mochii {
Ref<spdlog::logger> Log::s_CoreLogger;
Ref<spdlog::logger> Log::s_ClientLogger;

void Log::Init() {
  spdlog::set_pattern("%^[%T] %n: %v%$");

  auto coreLogger = spdlog::get("MOCHII");
  if (!coreLogger) {
    coreLogger = spdlog::stdout_color_mt("MOCHII");
  }
  s_CoreLogger = coreLogger;
  s_CoreLogger->set_level(spdlog::level::trace);

  auto clientLogger = spdlog::get("APP");
  if (!clientLogger) {
    clientLogger = spdlog::stdout_color_mt("APP");
  }
  s_ClientLogger = clientLogger;
  s_ClientLogger->set_level(spdlog::level::trace);
}
}  // namespace Mochii
