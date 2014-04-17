#ifndef ___INANITY_LOG_HPP___
#define ___INANITY_LOG_HPP___

#include "String.hpp"
#include <ostream>

BEGIN_INANITY

class Exception;

/// Global log.
/** Simplified log service for debugging and reporting errors. */
class Log
{
private:
	static Log* instance;

public:
	enum Severity
	{
		severityMessage,
		severityWarning,
		severityError
	};

public:
	/// Main write method.
	virtual void Write(Severity severity, const String& string) = 0;

	static void SetInstance(Log* log);

	//** Helper methods.
	static void Message(const String& string);
	static void Warning(const String& string);
	static void Error(const String& string);
	static void Error(ptr<Exception> exception);
};

/// Class logs to standard stream.
class StandardLog : public Log
{
public:
	std::ostream& stream;
	/// Instance which logs into std::cerr.
	static StandardLog cerrInstance;

public:
	StandardLog(std::ostream& stream);

	//*** Log's methods.
	void Write(Severity severity, const String& string);
};

END_INANITY

#endif
