#include "ShaderCache.hpp"
#include "Device.hpp"
#include "ShaderCompiler.hpp"
#include "ShaderSource.hpp"
#include "VertexShader.hpp"
#include "PixelShader.hpp"
#include "shaders/ShaderGenerator.hpp"
#include "shaders/Expression.hpp"
#include "shaders/Node.hpp"
#include "../File.hpp"
#include "../FileSystem.hpp"
#include "../crypto/HashStream.hpp"
#include "../File.hpp"
#include "../Exception.hpp"

ShaderCache::ShaderCache(ptr<FileSystem> fileSystem, ptr<Device> device, ptr<ShaderCompiler> shaderCompiler, ptr<ShaderGenerator> shaderGenerator, ptr<HashStream> hashStream)
: fileSystem(fileSystem), device(device), shaderCompiler(shaderCompiler), shaderGenerator(shaderGenerator), hashStream(hashStream)
{}

String ShaderCache::CalculateHash(ptr<ShaderSource> shaderSource)
{
	hashStream->Reset();
	shaderSource->Serialize(hashStream);
	hashStream->End();

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
	try
	{
		return device->CreateVertexShader(GetShader(shaderSource));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get vertex shader", exception);
	}
}

ptr<VertexShader> ShaderCache::GetVertexShader(Expression shaderExpression)
{
	try
	{
		return GetVertexShader(shaderGenerator->Generate(shaderExpression, ShaderTypes::vertex));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get vertex shader from expression", exception);
	}
}

ptr<PixelShader> ShaderCache::GetPixelShader(ptr<ShaderSource> shaderSource)
{
	try
	{
		return device->CreatePixelShader(GetShader(shaderSource));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get pixel shader", exception);
	}
}

ptr<PixelShader> ShaderCache::GetPixelShader(Expression shaderExpression)
{
	try
	{
		return GetPixelShader(shaderGenerator->Generate(shaderExpression, ShaderTypes::pixel));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't get pixel shader from expression", exception);
	}
}
