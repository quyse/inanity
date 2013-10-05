(function TestFunc(){
	TestClass.print(JSON.stringify({a:'b',c:"ddd"}), new RegExp("abc[d-f]+"));
	TestClass.print(Inanity.FolderFileSystem.GetNativeFileSystem().LoadFile("script/v8/test.js").GetSize(), "ddd");
	TestClass.print(JSON.stringify(this), "ddd");

	var test = new TestClass();
	var m = [
		1,2,3,4,
		5,6,7,8,
		9,10,11,12,
		13,14,15,16];

	test.printvec3(m);
	test.printvec4(m);
	test.printmat4x4(m);

	test.work("a", 12.23, 323.32);
	test.work("a", 12.23, 323.32);
	//TestClass.print(Inanity.File());
	//throw "Fuck!";
}).apply();
