#ifndef ___INANITY_GRAPHICS_SHADERS_SWIZZLE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SWIZZLE_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

/// Узел, выполняющий перемешивание (swizzle) компонентов вектора.
class SwizzleNode : public ValueNode
{
private:
	ptr<ValueNode> a;
	/// Строка, задающая перемешивание.
	/** Содержит от 1 до 4 символов x, y, z, w.
	Должна быть литералом, так как память не управляется классом. */
	const char* map;

public:
	SwizzleNode(const char* map, ptr<ValueNode> a);

	const char* GetMap() const;
	ptr<ValueNode> GetA() const;
};

END_INANITY_SHADERS

#endif
