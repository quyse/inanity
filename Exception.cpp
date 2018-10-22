#include "Exception.hpp"
#include "Strings.hpp"

#if defined(___INANITY_PLATFORM_WINDOWS)
#include "platform/windows.hpp"
#elif defined(___INANITY_PLATFORM_POSIX)
#include <errno.h>
// для strerror
#include <cstring>
#else
#error Unknown platform
#endif

BEGIN_INANITY

Exception::Exception(const String& message) : message(message)
{
}

Exception::Exception(const String& message, ptr<Exception> innerException) : message(message), innerException(innerException)
{
}

String Exception::GetMessageText() const
{
	return message;
}

ptr<Exception> Exception::GetInnerException() const
{
	return innerException;
}

void Exception::PrintStack(std::ostream& stream) const
{
	stream << message;
	if(innerException)
	{
		stream << " BECAUSE ";
		innerException->PrintStack(stream);
	}
}

ptr<Exception> Exception::SystemError()
{
#if defined(___INANITY_PLATFORM_WINDOWS)
	int errorCode = GetLastError();
#elif defined(___INANITY_PLATFORM_POSIX)
	int errorCode  = errno;
#else
#error Unknown platform
#endif

	return SystemError(errorCode);
}

ptr<Exception> Exception::SystemError(int errorCode)
{
#if defined(___INANITY_PLATFORM_WINDOWS)

#if defined(___INANITY_PLATFORM_XBOX)
	wchar_t buffer[1024];
	if(!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, 0, (wchar_t*)&buffer, sizeof(buffer) / sizeof(buffer[0]), 0))
#else
	wchar_t* buffer;
	if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, 0, (wchar_t*)&buffer, 0, 0))
#endif
		return NEW(Exception("Unknown system error"));

	ptr<Exception> exception = NEW(Exception(Strings::Unicode2UTF8(buffer)));
#if !defined(___INANITY_PLATFORM_XBOX)
	LocalFree(buffer);
#endif

	return exception;

#elif defined(___INANITY_PLATFORM_POSIX)

	return NEW(Exception(strerror(errorCode)));

#else

#error Unknown platform

#endif
}

END_INANITY
