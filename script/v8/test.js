(function TestFunc(){
	TestClass.print(JSON.stringify({a:'b',c:"ddd"}), new RegExp("abc[d-f]+"));
	TestClass.print(Inanity.FolderFileSystem.GetNativeFileSystem().LoadFile("script/v8/test.js").GetSize(), "ddd");
	TestClass.print(JSON.stringify(this), "ddd");

	var test = new TestClass();
	test.work("a", 12.23, 323.32);
	test.work("a", 12.23, 323.32);
	//TestClass.print(Inanity.File());
	//throw "Fuck!";
}).apply();
