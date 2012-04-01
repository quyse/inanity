#ifndef ___INANITY_DX_TEXTURE_HPP___
#define ___INANITY_DX_TEXTURE_HPP___

#include "dx.hpp"
#include "d3d.hpp"
#include "../../ComPointer.hpp"

BEGIN_INANITY

class File;
class ResourceLoader;

END_INANITY

BEGIN_INANITY_DX

/// Класс текстуры.
class Texture : public Object
{
private:
	ComPointer<ID3D11ShaderResourceView> shaderResourceView;

public:
	Texture(ID3D11ShaderResourceView* shaderResourceView);

	ID3D11ShaderResourceView* GetShaderResource() const;

	static ptr<Texture> Deserialize(ptr<File> file, ResourceLoader* resourceLoader);
};

END_INANITY_DX

#endif
