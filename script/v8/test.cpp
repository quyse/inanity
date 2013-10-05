#include "State.hpp"
#include "../Function.hpp"
#include "../../FolderFileSystem.hpp"
#include <iostream>
using namespace Inanity;

#include "impl.ipp"
#include "../../inanity-base-meta.ipp"

Script::State* globalState;

class TestClass : public Object
{
public:
	static void print(const String& a, const String& b)
	{
		std::cout << "Test print: " << a << ", " << b << '\n';
	}

	ptr<TestClass> work(const String& a, int b, double c)
	{
		std::cout << "Work: " << a << ' ' << b << ' ' << c << '\n';
		globalState->ReclaimInstance(this);
		return this;
	}

	META_DECLARE_CLASS(TestClass);
};

META_CLASS(TestClass, TestClass);
	META_CONSTRUCTOR();
	META_STATIC_METHOD(print);
	META_METHOD(work);
META_CLASS_END();

int main(int argc, char* argv[])
{
	try
	{
		ptr<Script::V8::State> state = NEW(Script::V8::State());
		globalState = state;
		state->Register<FolderFileSystem>();
		state->Register<TestClass>();
		state->LoadScript(FolderFileSystem::GetNativeFileSystem()->LoadFile("script/v8/test.js"))->Run();
	}
	catch(Exception* exception)
	{
		MakePointer(exception)->PrintStack(std::cout);
	}

	return 0;
}
