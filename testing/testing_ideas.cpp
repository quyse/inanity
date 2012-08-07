#include <cstdio>

template <typename ClassType>
class TestFixture
{
public:
	TestFixture();

	void Run() { TestFixtureTests<ClassType>::Run(); }
};

template <typename ClassType>
class TestFixtureTests
{
	void Run();
};

#define TEST_FIXTURE_TESTS_BEGIN(className) \
	template <> TestFixtureTests<className>::Run() {
		TestRunner runner(#className); \
#define TEST_FIXTURE_TESTS_END() }
#define TEST_FIXTURE_TEST_BEGIN(testName, expected) \
	struct Test_##testName { \
		static void Run(TestRunner& runner) { \
			{ \
				try { \
					TEST_EXPECTED_##expected##_BEGIN(); \
					Test_##testName::Run(); \
				} catch(Exception* exception) { \
#define TEST_FIXTURE_TEST_END() \
				TEST_EXPECTED_##expected##_END(); \
			} \
		} \
	}; \
	runner.BeginTest(#testName); \
		TEST_FAIL("unexpected exception: " + MakePointer(exception)->GetStackText()); \
	} \
	runner.EndTest()

#define TEST_OK(msg) \
	return runner.Ok(msg)
#define TEST_FAIL(msg) \
	return runner.Fail(msg)

#define TEST_EXPECTED_OK_BEGIN() \
	runner.expected("ok")
#define TEST_EXPECTED_OK_END()
#define TEST_EXPECTED_EXCEPTION_BEGIN() \
	try {
#define TEST_EXPECTED_EXCEPTION_END() \
		TEST_FAIL("normal finish"); \
	} catch(Exception* exception) { \
		MakePointer(exception)->GetStackText(); \
		TEST_OK("got exception: " + stream.str());
	}


class Leonid
{
};

TEST_FIXTURE_TESTS_BEGIN(Leonid);
	TEST_FIXTURE_TEST_BEGIN(SpartaScream, EXCEPTION);
		Leonid leonid;
		leonid.ThisIsSparta();
	TEST_FIXTURE_TEST_END();
TEST_FIXTURE_TESTS_END();

int main()
{
	return 0;
}
