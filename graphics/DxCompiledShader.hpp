#ifndef ___INANITY_GRAPHICS_DX_COMPILED_SHADER_HPP___
#define ___INANITY_GRAPHICS_DX_COMPILED_SHADER_HPP___

#include "DxShaderResources.hpp"

BEGIN_INANITY

class File;
class OutputStream;
class InputStream;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс скомпилированного шейдера DirectX.
class DxCompiledShader : public Object
{
private:
	ptr<File> code;
	DxShaderResources resources;

public:
	DxCompiledShader(ptr<File> code, const DxShaderResources& resources);

	ptr<File> GetCode() const;
	const DxShaderResources& GetResources() const;

	void Serialize(ptr<OutputStream> outputStream);
	static ptr<DxCompiledShader> Deserialize(ptr<InputStream> inputStream);
};

END_INANITY_GRAPHICS

#endif
