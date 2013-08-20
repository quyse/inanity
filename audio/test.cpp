#include "OggVorbisSource.hpp"
#include "AlSystem.hpp"
#include "Device.hpp"
#include "Sound.hpp"
#include "Player.hpp"
#include "../FolderFileSystem.hpp"
#include "../File.hpp"
#include "../InputStream.hpp"
#include "../OutputStream.hpp"
#include "../Exception.hpp"
#include <iostream>
#include <unistd.h>
using namespace Inanity;

int main(int argc, char** argv)
{
	try
	{
		argc--;
		argv++;

		if(argc == 1)
		{
			ptr<FileSystem> fileSystem = FolderFileSystem::GetNativeFileSystem();
			ptr<File> sourceFile = fileSystem->LoadFile(argv[0]);

			ptr<Audio::System> system = NEW(Audio::AlSystem());
			ptr<Audio::Device> device = system->CreateDefaultDevice();
			ptr<Audio::Sound> sound = device->CreateBufferedSound(NEW(Audio::OggVorbisSource(fileSystem->LoadFile(argv[0]))));
			ptr<Audio::Player> player = sound->CreatePlayer();
			player->Play();
			do
			{
				system->Tick();
				sleep(0);
			}
			while(player->IsPlaying());
		}

		if(argc == 2)
		{
			ptr<FileSystem> fileSystem = FolderFileSystem::GetNativeFileSystem();
			ptr<File> sourceFile = fileSystem->LoadFile(argv[0]);
			ptr<OutputStream> destStream = fileSystem->SaveStream(argv[1]);

			ptr<Audio::Source> source = NEW(Audio::OggVorbisSource(sourceFile));
			destStream->ReadAllFromStream(source->CreateStream());
		}
	}
	catch(Exception* exception)
	{
		MakePointer(exception)->PrintStack(std::cout);
	}

	return 0;
}
