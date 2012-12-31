#ifndef ___INANITY_GRAPHICS_SHADERS_TEMP_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_TEMP_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

/// Класс временной переменной в шейдере.
class TempNode : public Node
{
private:
	DataType valueType;

public:
	TempNode(DataType valueType);

	Type GetType() const;

	DataType GetValueType() const;
};

END_INANITY_SHADERS

#endif
