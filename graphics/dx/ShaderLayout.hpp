#ifndef ___INANITY_SHADER_LAYOUT_HPP___
#define ___INANITY_SHADER_LAYOUT_HPP___

#include "dx.hpp"
#include "../../ComPointer.hpp"
#include "d3d.hpp"

BEGIN_INANITY_DX

/// Класс разметки данных шейдера (layout).
/** Представляет собой класс-обертку над соответствующим интерфейсом DirectX. */
class ShaderLayout : public Object
{
private:
	/// Интерфейс DirectX.
	ComPointer<ID3D11InputLayout> layout;

public:
	/// Создать разметку данных шейдера.
	ShaderLayout(ID3D11InputLayout* layout);

	/// Получить интерфейс.
	ID3D11InputLayout* GetInternalInterface() const;
};

END_INANITY_DX

#endif
