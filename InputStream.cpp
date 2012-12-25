#include "InputStream.hpp"
#include "File.hpp"
#include "Future.hpp"
#include "scripting_impl.hpp"

SCRIPTABLE_MAP_BEGIN(InputStream, Inanity.InputStream);
SCRIPTABLE_MAP_END();

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
