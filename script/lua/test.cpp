#include "../../inanity-base.hpp"
#include "../../inanity-lua.hpp"
#include "../../inanity-platform.hpp"

#include "impl.ipp"
#include "../../inanity-base-meta.ipp"

#include <iostream>
#include <sstream>

using namespace Inanity;

class TestClass : public Object
{
public:
	TestClass();

	static void print(ptr<Script::Any> any);
	static String isflags(ptr<Script::Any> any);

	META_DECLARE_CLASS(TestClass);
};

META_CLASS(TestClass, TestClass);
	META_CONSTRUCTOR();
	META_STATIC_METHOD(print);
	META_STATIC_METHOD(isflags);
META_CLASS_END();

TestClass::TestClass() {}

void TestClass::print(ptr<Script::Any> any)
{
	any->Dump(std::cout);
	std::cout << "\n";
}

String TestClass::isflags(ptr<Script::Any> any)
{
	std::ostringstream ss;
	ss
		<< (any->IsNull() ? "(null)" : "")
		<< (any->IsBoolean() ? "(boolean)" : "")
		<< (any->IsNumber() ? "(number)" : "")
		<< (any->IsString() ? "(string)" : "")
		<< (any->IsArray() ? "(array)" : "")
		<< (any->IsFunction() ? "(function)" : "")
		<< (any->IsObject() ? "(object)" : "");
	return ss.str();
}

static void Run()
{
	try
	{
		ptr<Script::Lua::State> state = NEW(Script::Lua::State());

		state->Register<TestClass>();

		ptr<Script::Function> function = state->LoadScript(Platform::FileSystem::GetNativeFileSystem()->LoadFile("script/lua/test.lua"));

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
