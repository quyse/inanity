#ifndef ___INANITY_DX_TEXTURE_HPP___
#define ___INANITY_DX_TEXTURE_HPP___

#include "dx.hpp"
#include "d3d.hpp"
#include "../../ComPointer.hpp"

BEGIN_INANITY_DX

/*
Файл содержит класс текстуры. Текстура - это ресурс,
загружаемый загрузчиком TextureLoader.
*/

class Texture : public Object
{
private:
	ComPointer<ID3D11ShaderResourceView> shaderResourceView;

public:
	Texture(ID3D11ShaderResourceView* shaderResourceView);

	ID3D11ShaderResourceView* GetShaderResource() const;
};

END_INANITY_DX

#endif
