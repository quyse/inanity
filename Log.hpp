#ifndef ___INANITY_LOG_HPP___
#define ___INANITY_LOG_HPP___

#include "String.hpp"
#include "Exception.hpp"
#include <sstream>

BEGIN_INANITY

/// Global log.
/** Simplified log service for debugging and reporting errors. */
class Log
{
private:
	static Log* instance;

protected:
	template <typename... T>
	struct ArgWriter;

	template <typename T>
	struct ThingWriter;

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

	/// Handy template methods.
	template <typename... Args>
	void Write(Severity severity, Args... args)
	{
		std::ostringstream stream;
		ArgWriter<Args...>::Write(stream, args...);
		Write(severity, stream.str());
	}

	static void SetInstance(Log* log);

	//** Helper methods.
	static void Message(const String& string);
	template <typename... Args>
	static void Message(Args... args)
	{
		instance->Write(severityMessage, args...);
	}
	static void Warning(const String& string);
	template <typename... Args>
	static void Warning(Args... args)
	{
		instance->Write(severityWarning, args...);
	}
	static void Error(const String& string);
	template <typename... Args>
	static void Error(Args... args)
	{
		instance->Write(severityError, args...);
	}
};

template <typename First, typename... Rest>
struct Log::ArgWriter<First, Rest...>
{
	static void Write(std::ostringstream& stream, First first, Rest... rest)
	{
		ThingWriter<First>::Write(stream, first);
		ArgWriter<Rest...>::Write(stream, rest...);
	}
};
template <>
struct Log::ArgWriter<>
{
	static void Write(std::ostringstream&) {}
};

template <typename T>
struct Log::ThingWriter
{
	static void Write(std::ostream& stream, T t)
	{
		stream << t;
	}
};
template <>
struct Log::ThingWriter<ptr<Exception> >
{
	static void Write(std::ostream& stream, ptr<Exception> exception)
	{
		exception->PrintStack(stream);
	}
};
template <>
struct Log::ThingWriter<Exception*>
{
	static void Write(std::ostream& stream, Exception* exception)
	{
		ThingWriter<ptr<Exception> >::Write(stream, exception);
	}
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
