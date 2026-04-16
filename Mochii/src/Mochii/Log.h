#pragma once
#include <memory>
#include "Core.h"
#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Mochii {
	class MOCHII_API Log {
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return _CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _ClientLogger;  }
	private:
		static std::shared_ptr<spdlog::logger> _CoreLogger;
		static std::shared_ptr<spdlog::logger> _ClientLogger;
	};
}

