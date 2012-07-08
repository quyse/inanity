#include "test.hpp"
#include "../inanity-base.hpp"
#include "../scripting.hpp"
#include <iostream>
#include <sstream>

BEGIN_INANITY_LUA

SCRIPTABLE_MAP_BEGIN(ClassA, Inanity.Lua.ClassA);
	SCRIPTABLE_CONSTRUCTOR(ClassA);
	SCRIPTABLE_METHOD(ClassA, print);
	SCRIPTABLE_METHOD(ClassA, print2);
	SCRIPTABLE_METHOD(ClassA, describe);
SCRIPTABLE_MAP_END();
SCRIPTABLE_MAP_BEGIN(ClassB, Inanity.Lua.ClassB);
	SCRIPTABLE_PARENT(ClassA);
	SCRIPTABLE_CONSTRUCTOR(ClassB, ptr<ClassA>);
	SCRIPTABLE_METHOD(ClassB, print3);
SCRIPTABLE_MAP_END();

ClassA::ClassA() {}

void ClassA::print(const String& a)
{
	std::cout << "ClassA:print: " << a << '\n';
}

void ClassA::print2(int a, const String& b)
{
	std::cout << "ClassA::print2: " << a << ", " << b << '\n';
}

String ClassA::describe(ptr<ClassA> a)
{
	char str[10];
	sprintf(str, "%p", (ClassA*)a);
	return str;
}

ClassB::ClassB(ptr<ClassA> a) : a(a) {}

void ClassB::print(const String& a)
{
	std::cout << "ClassB:print: " << a << '\n';
}

void ClassB::print3(ptr<ClassA> a)
{
	std::cout << "ClassB::print3: " << (ClassA*)a << '\n';
}

END_INANITY_LUA

int main()
{
	try
	{
		ptr<ScriptState> state = NEW(ScriptState());

		state->RegisterClass<Lua::ClassA>();
		state->RegisterClass<Lua::ClassB>();

		ptr<Script> script = state->LoadScript(FolderFileSystem::GetNativeFileSystem()->LoadFile("lua/test.lua"));

		script->Run<void>();
	}
	catch(Exception* exception)
	{
		std::ostringstream s;
		MakePointer(exception)->PrintStack(s);
		std::cout << s.str() << '\n';
	}

	return 0;
}
