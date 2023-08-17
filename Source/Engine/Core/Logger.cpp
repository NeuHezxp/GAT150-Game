#include "Logger.h"
#include "FileIO.h"
#include <iostream>

namespace kiko
{

	// Implementation of the Log function of the Logger class.
	// This function logs the message with the specified log level, file name, and line number.
	bool Logger::Log(LogLevel logLevel, const std::string& filename, int line)
	{
		if (logLevel < m_logLevel)
			return false; ///If log level is less than current log level, return false

		// Switch statement to determine the log level and append the corresponding prefix to the message.
		switch (logLevel)
		{
		case LogLevel::Info:
			*this << "INFO: ";
			break;
		case LogLevel::Warning:
			*this << "WARNING: ";
			break;
		case LogLevel::Error:
			*this << "ERROR: ";
			break;
		case LogLevel::Assert:
			*this << "ASSERT: ";
			break;
		default:
			break;
		}

		// Append the file name and line number to the message.
		*this << getFileName(filename) << "(" << line << ") ";

		// Return true, indicating that the message has been logged.
		return true;
	}
}