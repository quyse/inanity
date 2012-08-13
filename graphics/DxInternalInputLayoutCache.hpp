#ifndef ___INANITY_GRAPHICS_DX_INTERNAL_INPUT_LAYOUT_CACHE_HPP___
#define ___INANITY_GRAPHICS_DX_INTERNAL_INPUT_LAYOUT_CACHE_HPP___

#include "graphics.hpp"
#include <unordered_map>

BEGIN_INANITY_GRAPHICS

class VertexLayout;
class DxVertexShader;
class DxContext;
class DxInternalInputLayout;

/// Класс кэша входных разметок DirectX.
/** Сопоставляет разметки вершин (VertexLayout) с вершинными шейдерами. */
class DxInternalInputLayoutCache : public Object
{
private:
	struct Key
	{
		VertexLayout* vertexLayout;
		DxVertexShader* vertexShader;
		Key(VertexLayout* vertexLayout, DxVertexShader* vertexShader);
		operator size_t() const;
	};
	typedef std::unordered_map<Key, ptr<DxInternalInputLayout> > InputLayouts;
	InputLayouts inputLayouts;

	DxContext* context;

public:
	DxInternalInputLayoutCache(DxContext* context);

	ptr<DxInternalInputLayout> GetInputLayout(VertexLayout* vertexLayout, DxVertexShader* vertexShader);
};

END_INANITY_GRAPHICS

#endif
