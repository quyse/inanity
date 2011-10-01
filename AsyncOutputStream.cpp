#include "AsyncOutputStream.hpp"
#include "File.hpp"
#include "Exception.hpp"

AsyncOutputStream::EventData::EventData(EventType type, ptr<Exception> exception) :
	type(type), exception(exception)
{
}

AsyncOutputStream::AsyncOutputStream(ptr<Handler> handler) :
	handler(handler), ready(true)
{
}

void AsyncOutputStream::ProcessQueue()
{
	// если поток не готов принимать данные
	if (!ready)
		return;

	// если очередь пуста
	if (bufferedFiles.empty())
	{
		// сообщить об этом
		handler->Fire(EventData(eventTypeFlushed));
		return;
	}

	ready = false;
	ptr<File> file = bufferedFiles.front();
	bufferedFiles.pop();
	WriteData(file->GetData(), file->GetSize());
}

void AsyncOutputStream::HandleReady()
{
	ready = true;
	ProcessQueue();
}

void AsyncOutputStream::HandleException(ptr<Exception> exception)
{
	handler->Fire(EventData(eventTypeError, exception));
}

void AsyncOutputStream::Write(ptr<File> file)
{
	bufferedFiles.push(file);
	ProcessQueue();
}
