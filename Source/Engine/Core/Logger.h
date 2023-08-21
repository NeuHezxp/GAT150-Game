#pragma once

#include <string>
#include <cassert>
#include <fstream>
#include <iostream>

#include "../Framework/Singleton.h"

// Macros / debug mode
#ifdef _DEBUG
// INFO_LOG macro logs information messages
#define INFO_LOG(message) { if (kiko::Logger::Instance().Log(kiko::LogLevel::Info, __FILE__, __LINE__)) { kiko::Logger::Instance() << message << "\n"; } }
// WARNING_LOG macro logs warning messages
#define WARNING_LOG(message) { if (kiko::Logger::Instance().Log(kiko::LogLevel::Warning, __FILE__, __LINE__)) { kiko::Logger::Instance() << message << "\n"; } }
// ERROR_LOG macro logs error messages
#define ERROR_LOG(message) { if (kiko::Logger::Instance().Log(kiko::LogLevel::Error, __FILE__, __LINE__)) { kiko::Logger::Instance() << message << "\n"; } }
// ASSERT_LOG macro logs assert messages and triggers an assertion if the condition is false
#define ASSERT_LOG(condition, message) { if (!condition && kiko::Logger::Instance().Log(kiko::LogLevel::Assert, __FILE__, __LINE__)) { kiko::Logger::Instance() << message << "\n"; } assert(condition); }
#else
// If not in debug mode, define these macros as empty to disable logging
#define INFO_LOG(message) {}
#define WARNING_LOG(message) {}
#define ERROR_LOG(message) {}
#define ASSERT_LOG(condition, message) {}
#endif // _DEBUG

namespace kiko
{
	// Enum class defining different log levels
	enum class LogLevel
	{
		Info,    // Informational messages
		Warning, // Warning messages
		Error,   // Error messages
		Assert   // Assertion failure messages
	};

	// Logger class for handling logging messages
	class Logger : public Singleton<Logger> // makes class singleton
	{
	public:

		// Constructor with log level, output stream, and an optional filename for file logging
		Logger(LogLevel logLevel = LogLevel::Info, std::ostream* ostream = &std::cout, const std::string& filename = "log.txt") :
			m_logLevel{ logLevel },
			m_ostream{ ostream }
		{
			// If a filename is provided, open a file stream for file logging
			if (!filename.empty()) m_fstream.open(filename);
		}

		// Log a message with the specified log level, filename, and line number
		bool Log(LogLevel logLevel, const std::string& filename, int line);

		// Operator overload for streaming messages to the logger
		template<typename T>
		Logger& operator << (T value);

	private:
		LogLevel m_logLevel;       // Log level for the logger
		std::ostream* m_ostream;   // Output stream for console logging
		std::ofstream m_fstream;   // File stream for file logging (optional)
	};

	// Operator overload for streaming messages to the logger
	template<typename T>
	inline Logger& Logger::operator << (T value)
	{
		// If console output stream exists, stream the value to it
		if (m_ostream) *m_ostream << value;

		// If file stream is open, stream the value to the file and flush the buffer
		if (m_fstream.is_open())
		{
			m_fstream << value;
			m_fstream.flush();
		}

		return *this;
	}
}
