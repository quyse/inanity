-- Тестовый скрипт

local a = Inanity.Lua.ClassB(Inanity.Lua.ClassA())
a:print("this is a test string")
a:print2(1234, "fdsfe")

local fileSystem = Inanity.FolderFileSystem.GetNativeFileSystem();

-- Inanity.Lua.ClassA.printFile(fileSystem:LoadFile("lua/test.lua"))

local bfsStream = Inanity.CompressStream.CreateMax(fileSystem:SaveStream("lua/test.bfs"));
local bfsBuilder = Inanity.BlobFileSystemBuilder(bfsStream);
bfsBuilder:AddFile("gdfsgfd", fileSystem:LoadFile("lua/test.lua"), 4);
bfsBuilder:Finalize();
