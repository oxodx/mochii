#pragma once
#include "mzpch.h"
#include "Core.h"
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

// Core log macros
#define MI_CORE_TRACE(...)	::Mochii::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MI_CORE_INFO(...)	::Mochii::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MI_CORE_WARN(...)	::Mochii::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MI_CORE_ERROR(...)	::Mochii::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MI_CORE_FATAL(...)	::Mochii::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define MI_TRACE(...)		::Mochii::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MI_INFO(...)		::Mochii::Log::GetClientLogger()->info(__VA_ARGS__)
#define MI_WARN(...)		::Mochii::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MI_ERROR(...)		::Mochii::Log::GetClientLogger()->error(__VA_ARGS__)
#define MI_FATAL(...)		::Mochii::Log::GetClientLogger()->fatal(__VA_ARGS__)
