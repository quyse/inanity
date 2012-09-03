#include "InputStream.hpp"
#include "File.hpp"
#include "Future.hpp"

ptr<Future<size_t> > InputStream::ReadAsync(ptr<File> file)
{
	ptr<Future<size_t> > future = NEW(Future<size_t>());
	try
	{
		future->Result(Read(file->GetData(), file->GetSize()));
	}
	catch(Exception* exception)
	{
		future->Error(exception);
	}
	return future;
}
