#include "Exception.hpp"

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
		stream << L"    ---- BECAUSE OF ----\n";
		innerException->PrintStack(stream);
	}
}
