#ifndef ___INANITY_GRAPHICS_SHADER_CACHE_HPP___
#define ___INANITY_GRAPHICS_SHADER_CACHE_HPP___

#include "graphics.hpp"
#include "shaders/shaders.hpp"
#include "../crypto/crypto.hpp"
#include "../String.hpp"

BEGIN_INANITY

class FileSystem;
class File;

END_INANITY

BEGIN_INANITY_CRYPTO

class HashStream;

END_INANITY_CRYPTO

BEGIN_INANITY_SHADERS

class ShaderGenerator;
class Expression;

END_INANITY_SHADERS

BEGIN_INANITY_GRAPHICS

class Device;
class ShaderCompiler;
class ShaderSource;
class VertexShader;
class PixelShader;

/// Класс кэша шейдеров.
class ShaderCache : public Object
{
private:
	/// Файловая система для шейдеров.
	ptr<FileSystem> fileSystem;
	/// Графическое устройство.
	ptr<Device> device;
	/// Компилятор шейдеров.
	ptr<ShaderCompiler> shaderCompiler;
	/// Генератор шейдеров.
	ptr<Shaders::ShaderGenerator> shaderGenerator;

	/// Хеширующий поток.
	ptr<Crypto::HashStream> hashStream;

	/// Вычислить хеш шейдера.
	String CalculateHash(ptr<ShaderSource> shaderSource);

public:
	ShaderCache(ptr<FileSystem> fileSystem, ptr<Device> device, ptr<ShaderCompiler> shaderCompiler, ptr<Shaders::ShaderGenerator> shaderGenerator, ptr<Crypto::HashStream> hashStream);

	/// Получить шейдер.
	/** Если его нет в кэше, он компилируется и добавляется в него. */
	ptr<File> GetShader(ptr<ShaderSource> shaderSource);

	/// Получить вершинный шейдер.
	ptr<VertexShader> GetVertexShader(ptr<ShaderSource> shaderSource);
	ptr<VertexShader> GetVertexShader(Shaders::Expression shaderExpression);
	/// Получить пиксельный шейдер.
	ptr<PixelShader> GetPixelShader(ptr<ShaderSource> shaderSource);
	ptr<PixelShader> GetPixelShader(Shaders::Expression shaderExpression);
};

END_INANITY_GRAPHICS

#endif
