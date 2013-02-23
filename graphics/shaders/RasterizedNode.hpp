#ifndef ___INANITY_GRAPHICS_SHADERS_RASTERIZED_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_RASTERIZED_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

/// Класс переменной после пиксельного шейдера.
/** Пока поддерживается только результирующий цвет.
Результирующая глубина не поддерживается. */
class RasterizedNode : public Node
{
private:
	DataType valueType;
	/// Номер рендертаргета.
	int target;

public:
	RasterizedNode(DataType valueType, int target);

	Type GetType() const;

	DataType GetValueType() const;
	int GetTarget() const;
};

END_INANITY_SHADERS

#endif
