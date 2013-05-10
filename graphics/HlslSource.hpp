#ifndef ___INANITY_GRAPHICS_HLSL_SOURCE_HPP___
#define ___INANITY_GRAPHICS_HLSL_SOURCE_HPP___

#include "ShaderSource.hpp"
#include "DxShaderResources.hpp"
#include "../String.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс исходного кода шейдера на языке HLSL.
class HlslSource : public ShaderSource
{
private:
	/// Исходный код на HLSL.
	ptr<File> code;
	/// Имя основной функции.
	String functionName;
	/// Профиль компиляции.
	String profile;
	/// Ресурсы шейдера.
	DxShaderResources resources;

public:
	HlslSource(ptr<File> code, const String& functionName, const String& profile, const DxShaderResources& resources);

	void Serialize(ptr<OutputStream> outputStream);

	ptr<File> GetCode() const;
	const String& GetFunctionName() const;
	const String& GetProfile() const;
	const DxShaderResources& GetResources() const;
};

END_INANITY_GRAPHICS

#endif
