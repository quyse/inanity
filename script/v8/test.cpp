#include "State.hpp"
#include "../Function.hpp"
#include "../../FolderFileSystem.hpp"
#include "../../inanity-math.hpp"
#include <iostream>
using namespace Inanity;

#include "impl.ipp"
#include "../../inanity-base-meta.ipp"
#include "../../inanity-math-script.ipp"

Script::State* globalState;

class TestClass : public Object
{
public:
	static void print(const String& s)
	{
		std::cout << s << '\n';
	}

	static String isflags(ptr<Script::Any> any)
	{
		std::ostringstream ss;
		ss
			<< (any->IsNull() ? "(null)" : "")
			<< (any->IsNumber() ? "(number)" : "")
			<< (any->IsArray() ? "(array)" : "")
			<< (any->IsFunction() ? "(function)" : "")
			<< (any->IsObject() ? "(object)" : "")
			<< '\n';
		return ss.str();
	}

	ptr<TestClass> work(const String& a, int b, double c)
	{
		std::cout << "Work: " << a << ' ' << b << ' ' << c << '\n';
		globalState->ReclaimInstance(this);
		return this;
	}

	void printvec3(const Math::vec3& a)
	{
		std::cout << a << '\n';
	}
	void printvec4(Math::vec4 a)
	{
		std::cout << a << '\n';
	}
	void printmat4x4(const Math::mat4x4& a)
	{
		std::cout << a << '\n';
	}

	Math::vec3 cross(const Math::vec3& a, const Math::vec3& b)
	{
		return Math::cross(a, b);
	}

	Math::vec4 sum(const Math::vec4& a, const Math::vec4& b)
	{
		return a + b;
	}

	Math::mat4x4 scaling(const Math::vec3& s)
	{
		return Math::CreateScalingMatrix(s);
	}

	ptr<Script::Any> createsomething(ptr<Script::Any> function, ptr<Script::Any> a, ptr<Script::Any> b)
	{
		ptr<Script::Any> r = function->Call(a, b);
		ptr<Script::Any> array = globalState->NewArray(3);
		array->Set(0, a);
		array->Set(1, b);
		array->Set(2, r);
		return array;
	}

	ptr<Script::Any> apply(ptr<Script::Any> function, ptr<Script::Any> thisValue)
	{
		return function->Apply(thisValue);
	}

	META_DECLARE_CLASS(TestClass);
};

META_CLASS(TestClass, TestClass);
	META_CONSTRUCTOR();
	META_STATIC_METHOD(print);
	META_STATIC_METHOD(isflags);
	META_METHOD(work);
	META_METHOD(printvec3);
	META_METHOD(printvec4);
	META_METHOD(printmat4x4);
	META_METHOD(cross);
	META_METHOD(sum);
	META_METHOD(scaling);
	META_METHOD(createsomething);
	META_METHOD(apply);
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
