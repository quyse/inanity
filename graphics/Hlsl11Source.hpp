#ifndef ___INANITY_GRAPHICS_HLSL11_SOURCE_HPP___
#define ___INANITY_GRAPHICS_HLSL11_SOURCE_HPP___

#include "ShaderSource.hpp"
#include "Dx11ShaderResources.hpp"
#include "../String.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс исходного кода шейдера на языке HLSL.
class Hlsl11Source : public ShaderSource
{
private:
	/// Исходный код на HLSL.
	ptr<File> code;
	/// Имя основной функции.
	String functionName;
	/// Профиль компиляции.
	String profile;
	/// Ресурсы шейдера.
	Dx11ShaderResources resources;

public:
	Hlsl11Source(ptr<File> code, const String& functionName, const String& profile, const Dx11ShaderResources& resources);

	void Serialize(ptr<OutputStream> outputStream);

	ptr<File> GetCode() const;
	const String& GetFunctionName() const;
	const String& GetProfile() const;
	const Dx11ShaderResources& GetResources() const;
};

END_INANITY_GRAPHICS

#endif
