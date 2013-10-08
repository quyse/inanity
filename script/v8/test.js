(function TestFunc(){
	TestClass.print(JSON.stringify({a:'b',c:"ddd"}), new RegExp("abc[d-f]+"));
	TestClass.print(Inanity.FolderFileSystem.GetNativeFileSystem().LoadFile("script/v8/test.js").GetSize(), "ddd");
	TestClass.print(JSON.stringify(this), "ddd");

	var testis = function(o) {
		TestClass.print(JSON.stringify([o, TestClass.isflags(o)]));
	};
	testis(false);
	testis(true);
	testis({a:'a', b:'b'});
	testis(testis);
	testis([1,2,3]);
	testis(2134);
	testis("fdsfsd");

	var test = new TestClass();
	testis(test);

	TestClass.test_creation(testis);

	var m = [
		1,2,3,4,
		5,6,7,8,
		9,10,11,12,
		13,14,15,16];

	test.printvec3(m);
	test.printvec4(m);
	test.printmat4x4(m);

	test.apply(function() {
		this.printvec3([0,1,2]);
	}, test);

	test.printvec3(test.cross([1, 2, 3], [4, 5, 6]));
	test.printvec4(test.sum([7, 6, 9, -3], [-4, 5, 2, 1]));
	test.printmat4x4(test.scaling(test.cross([1, 2, 3], [4, 5, 6])));

	TestClass.print(JSON.stringify(test.createsomething(function(a, b) {
		return a * b;
	}, 3, 4)));

	TestClass.print(JSON.stringify(function(a, b) {
		return a * b;
	}));

	test.work("a", 12.23, 323.32);
	test.work("a", 12.23, 323.32);
	//TestClass.print(Inanity.File());
	//throw "Fuck!";
}).apply();
