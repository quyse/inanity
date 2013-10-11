local testis = function(value)
	TestClass.print(value)
	TestClass.print(TestClass.isflags(value))
end

testis(false)
testis(true)
testis({ a = "a", b = "b" })
testis(testis)
testis({ 1, 2, 3})
testis(2134)
testis("fadfsgd")

TestClass.print({"a","b"})

local test = TestClass()
testis(test, "(object)")
