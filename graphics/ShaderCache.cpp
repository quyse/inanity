#include "ShaderCache.hpp"
#include "Device.hpp"
#include "ShaderCompiler.hpp"
#include "ShaderSource.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "../File.hpp"
#include "../FileSystem.hpp"
#include "../StreamWriter.hpp"
#include "../crypto/HashStream.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

ShaderCache::ShaderCache(ptr<FileSystem> fileSystem, ptr<Device> device, ptr<ShaderCompiler> shaderCompiler, ptr<HashStream> hashStream)
: fileSystem(fileSystem), device(device), shaderCompiler(shaderCompiler), hashStream(hashStream)
{}

String ShaderCache::CalculateHash(ptr<ShaderSource> shaderSource)
{
	StreamWriter writer(hashStream);
	hashStream->Reset();

	writer.WriteString(shaderSource->GetFunctionName());
	writer.WriteString(shaderSource->GetProfile());
	hashStream->WriteFile(shaderSource->GetCode());
	hashStream->Flush();

	return hashStream->GetHashString();
}

ptr<File> ShaderCache::GetShader(ptr<ShaderSource> shaderSource)
{
	try
	{
		// вычислить хэш исходника
		String hash = CalculateHash(shaderSource);

		// попробовать найти файл в кэше
		ptr<File> file = fileSystem->TryLoadFile(hash);
		if(file)
			return file;

		// файла в кэше нет, скомпилировать
		file = shaderCompiler->Compile(shaderSource);
		// и сохранить в файловую систему
		fileSystem->SaveFile(file, hash);

		return file;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get shader from shader cache", exception);
	}
}

ptr<VertexShader> ShaderCache::GetVertexShader(ptr<ShaderSource> shaderSource)
{
	return device->CreateVertexShader(GetShader(shaderSource));
}

ptr<PixelShader> ShaderCache::GetPixelShader(ptr<ShaderSource> shaderSource)
{
	return device->CreatePixelShader(GetShader(shaderSource));
}
