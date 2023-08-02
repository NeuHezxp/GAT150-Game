#pragma once
#include <string>
#include <cassert>
#include <fstream>

namespace kiko
{
	enum class LogLevel
	{
		Info,
		Warning,
		Error,
		Assert
	};

	class Logger
	{
	public:
		Logger(LogLevel logLevel ,std::ostream* ostream) :
			m_logLevel{logLevel},
		m_ostream{ ostream }
		{} // initializes
		bool Log(LogLevel logLevel, const std::string& filename, int line);
	private:
		LogLevel m_logLevel;
		std::ostream* m_ostream = nullptr;
		std::ofstream m_fstream;
	};

	extern Logger g_logger;
}
