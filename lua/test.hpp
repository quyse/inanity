/* Тестовый файл Lua.
*/

#include "../inanity-base.hpp"
#include "../scripting_decl.hpp"

BEGIN_INANITY_LUA

class ClassA : public Object
{
public:
	ClassA();

	virtual void print(const String& a);
	void print2(int a, const String& b);

	static void printFile(ptr<File> a);

	SCRIPTABLE_CLASS(ClassA);
};

class ClassB : public ClassA
{
private:
	ptr<ClassA> a;

public:
	ClassB(ptr<ClassA> a);

	void print(const String& a);
	void print3(ptr<ClassA> a);

	SCRIPTABLE_CLASS(ClassB);
};

END_INANITY_LUA
