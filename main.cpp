#include "Object.hpp"
#include "FolderFileSystem.hpp"
#include "InputStream.hpp"
#include "DiskInputStream.hpp"
#include "OutputStream.hpp"
#include "DiskOutputStream.hpp"
#include "Exception.hpp"
#include <cstdio>
#include <iostream>
using namespace Inanity;

class Test: public Object
{
private:
	ptr<Test> a;

public:
	Test(ptr<Test> a = 0) :
		a(a)
	{
	}

	void hello()
	{
		printf("aaa");
	}
};

int main(int argc, char** argv)
{
	argc--;
	argv++;

	try
	{
		if(argc < 2)
			THROW_PRIMARY_EXCEPTION("Not enough arguments");

		ptr<FileSystem> fs = FolderFileSystem::GetNativeFileSystem();
		fs->SaveFileAsStream(argv[2])->ReadAllFromStream(fs->LoadFileAsStream(argv[1]));
	}
	catch (Exception* exception)
	{
		MakePointer(exception)->PrintStack(std::cout);
	}

	return 0;
}
