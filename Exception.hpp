#ifndef ___INANITY_EXCEPTION_HPP___
#define ___INANITY_EXCEPTION_HPP___

#include "Object.hpp"
#include "String.hpp"
#include "meta/decl.hpp"
#include <ostream>

BEGIN_INANITY

/// Base exception class.
class Exception : public Object
{
protected:
	/// Text message about error.
	String message;
	ptr<Exception> innerException;

public:
	Exception(const String& message);
	Exception(const String& message, ptr<Exception> innerException);

	String GetMessageText() const;
	ptr<Exception> GetInnerException() const;

	virtual void PrintStack(std::ostream& stream) const;

	/// Create exception describes system error.
	/** System error is determined in a platform-specific way (Windows - GetLastError, Linux - errno). */
	static ptr<Exception> SystemError();
	static ptr<Exception> SystemError(int errorCode);

	META_DECLARE_CLASS(Exception);
};

#ifdef _DEBUG

#define __SLINE2__(x) #x
#define __SLINE3__(x) __SLINE2__(x)
#define __SLINE__ __SLINE3__(__LINE__)

/// Throw primary exception.
#define THROW(message) throw NEW(Inanity::Exception(Inanity::String("[ " __FILE__ ", " __SLINE__ " ] ") + (message)))
/// Throw secondary exception.
#define THROW_SECONDARY(message, exception) throw NEW(Inanity::Exception(Inanity::String("[ " __FILE__ ", " __SLINE__ " ] ") + (message), (exception)))
/// Throw exception in case of assert failure.
#define THROW_ASSERT(condition) \
	if(!(condition)) THROW(#condition)
/// New primary exception.
#define NEW_EXCEPTION(message) NEW(Inanity::Exception(Inanity::String("[ " __FILE__ ", " __SLINE__ " ] ") + (message)))
/// New secondary exception.
#define NEW_SECONDARY_EXCEPTION(message, exception) NEW(Inanity::Exception(Inanity::String("[ " __FILE__ ", " __SLINE__ " ] ") + (message), (exception)))

#else

#define THROW(message) throw NEW(Inanity::Exception((message)))
#define THROW_SECONDARY(message, exception) throw NEW(Inanity::Exception((message), (exception)))
#define THROW_ASSERT(condition)
#define NEW_EXCEPTION(message) NEW(Inanity::Exception((message)))
#define NEW_SECONDARY_EXCEPTION(message, exception) NEW(Inanity::Exception((message), (exception)))

#endif

#define BEGIN_TRY() try {
#define END_TRY(message) } catch(Inanity::Exception* exception) { THROW_SECONDARY(message, exception); }

END_INANITY

#endif
