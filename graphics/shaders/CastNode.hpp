#ifndef ___INANITY_GRAPHICS_SHADERS_CAST_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_CAST_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

/// Класс узла преобразования типа.
class CastNode : public Node
{
private:
	DataType castDataType;
	ptr<Node> a;

public:
	CastNode(DataType castDataType, ptr<Node> a);

	Type GetType() const;

	DataType GetCastDataType() const;
	ptr<Node> GetA() const;
};

END_INANITY_SHADERS

#endif
