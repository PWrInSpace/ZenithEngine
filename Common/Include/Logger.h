#pragma once
#include <string>

namespace ZE
{
	class Logger
	{
		enum class Level { Info, Warning, Error };

		static inline bool firstUse = true;

		static void Log(Level type, const std::string& log, bool noFile = false);

	public:
		Logger() = delete;

		static void InfoNoFile(const std::string& info) { Log(Level::Info, info, true); }
		static void Info(const std::string& info) { Log(Level::Info, info); }
		static void Warning(const std::string& warning) { Log(Level::Warning, warning); }
		static void Error(const std::string& error) { Log(Level::Error, error); }
	};
}