-- Тестовый скрипт

local a = Inanity.Lua.ClassB(Inanity.Lua.ClassA())
a:print("this is a test string")
a:print2(1234, "fdsfe")

Inanity.Lua.ClassA.printFile(Inanity.FolderFileSystem.GetNativeFileSystem():LoadFile("lua/test.lua"))
