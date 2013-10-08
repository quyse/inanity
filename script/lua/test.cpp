#include "../../inanity-base.hpp"
#include "../../inanity-compress.hpp"
#include "../../inanity-lua.hpp"

#include "impl.ipp"
#include "../../inanity-base-meta.ipp"
#include "../../inanity-compress-meta.ipp"

#include <iostream>
#include <sstream>

using namespace Inanity;

class ClassA : public Object
{
public:
	ClassA();

	virtual void print(const String& a);
	void print2(int a, const String& b);

	static void printFile(ptr<File> a);

	META_DECLARE_CLASS(ClassA);
};

class ClassB : public ClassA
{
private:
	ptr<ClassA> a;

public:
	ClassB(ptr<ClassA> a);

	void print(const String& a);
	void print3(ptr<ClassA> a);

	META_DECLARE_CLASS(ClassB);
};

META_CLASS(ClassA, ClassA);
	META_CONSTRUCTOR();
	META_METHOD(print);
	META_METHOD(print2);
	META_STATIC_METHOD(printFile);
META_CLASS_END();
META_CLASS(ClassB, ClassB);
	META_CLASS_PARENT(ClassA);
	META_CONSTRUCTOR(ptr<ClassA>);
	META_METHOD(print3);
META_CLASS_END();

ClassA::ClassA() {}

void ClassA::print(const String& a)
{
	std::cout << "ClassA::print: " << a << '\n';
}

void ClassA::print2(int a, const String& b)
{
	std::cout << "ClassA::print2: " << a << ", " << b << '\n';
}

void ClassA::printFile(ptr<File> file)
{
	std::cout << Strings::File2String(file);
}

ClassB::ClassB(ptr<ClassA> a) : a(a) {}

void ClassB::print(const String& a)
{
	std::cout << "ClassB::print: " << a << '\n';
}

void ClassB::print3(ptr<ClassA> a)
{
	std::cout << "ClassB::print3: " << (ClassA*)a << '\n';
}

static void Run()
{
	try
	{
		ptr<Script::Lua::State> state = NEW(Script::Lua::State());

		state->Register<ClassA>();
		state->Register<ClassB>();
		state->Register<FileSystem>();
		state->Register<File>();
		state->Register<FolderFileSystem>();
		state->Register<BlobFileSystem>();
		state->Register<BlobFileSystemBuilder>();
		state->Register<CompressStream>();
		state->Register<DecompressStream>();

		ptr<Script::Function> function = state->LoadScript(FolderFileSystem::GetNativeFileSystem()->LoadFile("script/lua/test.lua"));

		function->Run();
	}
	catch(Exception* exception)
	{
		std::ostringstream s;
		MakePointer(exception)->PrintStack(s);
		std::cout << s.str() << '\n';
	}
}

int main()
{
	Run();

	return 0;
}
