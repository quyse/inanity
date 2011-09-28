#include "Object.hpp"
#include "ManagedHeap.hpp"
#include <cstdio>
using namespace Inanity;

class Test : public Object
{
private:
	ptr<Test> a;

public:
	Test(ptr<Test> a = 0) : a(a) {}

	void hello()
	{
		printf("aaa");
	}
};

int main()
{
	ptr<Test> q = NEW(Test());
	ptr<Test> t = NEW(Test(q));

	t->hello();

	return 0;
}
