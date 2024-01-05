#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace XH
{
	class XH_API Log 
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

// Core log macros
#define XH_CORE_ERROR(...)		::XH::Log::GetCoreLogger()->error(__VA_ARGS__)
#define XH_CORE_INFO(...)		::XH::Log::GetCoreLogger()->info(__VA_ARGS__)
#define XH_CORE_WARN(...)		::XH::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define XH_CORE_TRACE(...)		::XH::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define XH_CORE_FATAL(...)		::XH::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define XH_ERROR(...)	::XH::Log::GetClientLogger()->error(__VA_ARGS__)
#define XH_INFO(...)		::XH::Log::GetClientLogger()->info(__VA_ARGS__)
#define XH_WARN(...)		::XH::Log::GetClientLogger()->warn(__VA_ARGS__)
#define XH_TRACE(...)	::XH::Log::GetClientLogger()->trace(__VA_ARGS__)
#define XH_FATAL(...)	::XH::Log::GetClientLogger()->fatal(__VA_ARGS__)