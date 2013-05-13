#ifndef ___INANITY_GRAPHICS_DX11_COMPILED_SHADER_HPP___
#define ___INANITY_GRAPHICS_DX11_COMPILED_SHADER_HPP___

#include "Dx11ShaderResources.hpp"

BEGIN_INANITY

class File;
class OutputStream;
class InputStream;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс скомпилированного шейдера DirectX.
class Dx11CompiledShader : public Object
{
private:
	ptr<File> code;
	Dx11ShaderResources resources;

public:
	Dx11CompiledShader(ptr<File> code, const Dx11ShaderResources& resources);

	ptr<File> GetCode() const;
	const Dx11ShaderResources& GetResources() const;

	void Serialize(ptr<OutputStream> outputStream);
	static ptr<Dx11CompiledShader> Deserialize(ptr<InputStream> inputStream);
};

END_INANITY_GRAPHICS

#endif
