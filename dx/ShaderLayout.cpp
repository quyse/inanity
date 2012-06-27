#include "ShaderLayout.hpp"

ShaderLayout::ShaderLayout(ID3D11InputLayout* layout) : layout(layout)
{
}

ID3D11InputLayout* ShaderLayout::GetInternalInterface() const
{
	return layout;
}
