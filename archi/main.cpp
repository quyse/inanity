#include "BlobCreator.hpp"
//#include "SimpleGeometryCreator.hpp"
//#include "FontCreator.hpp"
#include "SystemFontCreator.hpp"
#include "WavefrontObj.hpp"
//#include "XafConverter.hpp"
#include "SkeletonConverter.hpp"
#include "BoneAnimationConverter.hpp"
#include <string>
#include <iostream>
#include <sstream>

void PrintHelp()
{
	std::cout << "Inanity Archi: helps to create geometry, material and other Inanity's files\n\n";
	std::cout << "Using:\n\n";
	std::cout << "== Geometry ==\n";
	std::cout << "  wobj <source .obj> <result .geo> <format> // transform .obj geometry to Inanity geometry file\n";
	std::cout << "  possible formats:\n";
	std::cout << "    pnt (position3, normal3, texture2)\n    pbt (position3, bump3x3, texture2)\n";
	std::cout << "== Fonts ==\n";
	std::cout << "  font <source texture file> <source alphabat file> <char width> <char height> <result font file>";
	std::cout << "  systemfont <source alphabat file> <result texture file> <result font file> // create font with system font (it will be specified in select font dialog)";
	std::cout << "== Animation ==\n";
	std::cout << "  Converts 3dMAX *.xaf file to set of Inanity *.a files, one for each node in source file. Optionally can create a gear tree file for skeleton.";
	std::cout << "  xaf <source .xaf> <result .a> [<result .gt>]\n";
}

int wmain(int argc, const wchar_t** argv)
{
	ptr<Processor> processors[] =
	{
		NEW(BlobCreator()),
		NEW(WavefrontObj()),
//		NEW(SimpleGeometryCreator()),
		NEW(SystemFontCreator()),
		NEW(SkeletonConverter()),
		NEW(BoneAnimationConverter())
	};
	const size_t processorsCount = sizeof(processors) / sizeof(processors[0]);

	argc--;
	argv++;

	std::vector<String> arguments(argc);
	for(int i = 0; i < argc; ++i)
		arguments[i] = Strings::Unicode2UTF8(argv[i]);

	if(arguments.size() <= 0)
	{
		//вывести справку
		std::cout << "InanityArchi is command-line tool to perform some technical actions for Inanity.\n\n";
		std::cout << "Available commands:\n\n";
		for(size_t i = 0; i < processorsCount; ++i)
		{
			std::wcout << L" === Command: " << Strings::UTF82Unicode(processors[i]->GetCommand()) << L" ===\n";
			processors[i]->PrintHelp();
			std::wcout << L"\n";
		}
		return 0;
	}

	String command = arguments[0];
	arguments.erase(arguments.begin());
	for(size_t i = 0; i < processorsCount; ++i)
		if(processors[i]->GetCommand() == command)
		{
			try
			{
				processors[i]->Run(arguments);
			}
			catch(Exception* exception)
			{
				std::cout << "Exception occured: ";
				std::ostringstream stream;
				MakePointer(exception)->PrintStack(stream);
				std::wcout << Strings::UTF82Unicode(stream.str());
			}
			return 0;
		}
	std::cout << "Unknown command: " << command;
	return 0;
}
