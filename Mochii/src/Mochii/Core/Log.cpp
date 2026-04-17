#include "mzpch.h"
#include "Log.h"

namespace Mochii {
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		auto coreLogger = spdlog::get("MOCHII");
		if (!coreLogger) {
			coreLogger = spdlog::stdout_color_mt("MOCHII");
		}
		sCoreLogger = coreLogger;
		sCoreLogger->set_level(spdlog::level::trace);

		auto clientLogger = spdlog::get("APP");
		if (!clientLogger) {
			clientLogger = spdlog::stdout_color_mt("APP");
		}
		sClientLogger = clientLogger;
		sClientLogger->set_level(spdlog::level::trace);
	}
}
