#include "Exception.hpp"

Exception::Exception(const std::wstring& message) : message(message)
{
}

Exception::Exception(const std::wstring& message, ptr<Exception> innerException) : message(message), innerException(innerException)
{
}

std::wstring Exception::GetMessageText() const
{
	return message;
}

ptr<Exception> Exception::GetInnerException() const
{
	return innerException;
}

void Exception::PrintStack(std::wostream& stream) const
{
	//сначала печатаем собственное сообщение
	stream << message;
	//затем, если есть внутреннее исключение
	if(innerException)
	{
		//печатаем его
		stream << L"\n    ---- BECAUSE OF ----\n";
		innerException->PrintStack(stream);
	}
}
