#include "Exception.hpp"
#ifdef ___INANITY_LINUX
#include <errno.h>
// для strerror
#include <cstring>
#endif

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
	//сначала печатаем собственное сообщение
	stream << message << '\n';
	//затем, если есть внутреннее исключение
	if(innerException)
	{
		//печатаем его
		stream << "    ---- BECAUSE OF ----\n";
		innerException->PrintStack(stream);
	}
}

ptr<Exception> Exception::SystemError()
{
#ifdef ___INANITY_WINDOWS
	int errorCode = GetLastError();
#endif // ___INANITY_WINDOWS

#ifdef ___INANITY_LINUX
	int errorCode  = errno;
#endif // ___INANITY_LINUX

	return SystemError(errorCode);
}

ptr<Exception> Exception::SystemError(int errorCode)
{
#ifdef ___INANITY_WINDOWS

	wchar_t* buffer;
	if(!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, 0, &buffer, 0))
		return NEW(Exception("Unknown system error"));

	ptr<Exception> exception = NEW(Exception(buffer));
	LocalFree(buffer);

	return exception;

#endif // ___INANITY_WINDOWS

#ifdef ___INANITY_LINUX

	return NEW(Exception(strerror(errorCode)));

#endif // ___INANITY_LINUX
}
