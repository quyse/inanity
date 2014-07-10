using System;

namespace test
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			var generator = new Inanity.Script.Mono.AssemblyMeta ();
			generator.AddClass ("Inanity.File");
			generator.AddClassMethod("Inanity.File", "GetSize", "uint32", new string[] { });
			generator.AddClass("Inanity.Win32File");
			generator.SetClassParent("Inanity.Win32File", "Inanity.File");
			generator.SetClassConstructor("Inanity.Win32File", new string[]{ "string" });
			generator.Resolve("test", "test.dll");
		}
	}
}
