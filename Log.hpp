#ifndef ___INANITY_LOG_HPP___
#define ___INANITY_LOG_HPP___

#include "String.hpp"

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

/// Class logs to std::cerr.
class StandardLog : public Log
{
public:
	static StandardLog instance;

public:
	//*** Log's methods.
	void Write(Severity severity, const String& string);
};

END_INANITY

#endif
