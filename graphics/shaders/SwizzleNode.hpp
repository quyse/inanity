#ifndef ___INANITY_GRAPHICS_SHADERS_SWIZZLE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SWIZZLE_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

/// Узел, выполняющий перемешивание (swizzle) компонентов вектора.
class SwizzleNode : public Node
{
private:
	ptr<Node> a;
	/// Строка, задающая перемешивание.
	/** Содержит от 1 до 4 символов x, y, z, w.
	Должна быть литералом, так как память не управляется классом. */
	const char* map;

public:
	SwizzleNode(ptr<Node> a, const char* map);

	Type GetType() const;

	ptr<Node> GetA() const;
	const char* GetMap() const;
};

END_INANITY_SHADERS

#endif
