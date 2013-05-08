#ifndef ___INANITY_GRAPHICS_DX_ATTRIBUTE_BINDING_HPP___
#define ___INANITY_GRAPHICS_DX_ATTRIBUTE_BINDING_HPP___

#include "AttributeBinding.hpp"
#include "d3d.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

class AttributeLayout;
class DxVertexShader;

/// Класс привязки атрибутов для DirectX.
/** В данный момент класс ущербный, так как не хранит ID3D11InputLayout.
API не позволяет создать input layout с прямым указанием входных регистров.
Так что тут просто хранятся данные для создания разметки, и кэш таких
разметок хранится в контексте. */
class DxAttributeBinding : public AttributeBinding
{
private:
	// структуры описания
	std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
	// строки для имён семантик
	std::vector<String> semanticNames;

public:
	void Initialize(ptr<AttributeLayout> layout);

	ID3D11InputLayout* CreateInputLayout(ID3D11Device* deviceInterface, DxVertexShader* vertexShader);
};

END_INANITY_GRAPHICS

#endif
