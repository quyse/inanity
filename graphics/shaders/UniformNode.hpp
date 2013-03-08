#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_NODE_HPP___

#include "Node.hpp"

BEGIN_INANITY_SHADERS

class UniformGroup;

/// Класс узла Uniform-переменной.
class UniformNode : public Node
{
protected:
	/// Группа uniform-переменных.
	ptr<UniformGroup> group;
	/// Тип данных переменной.
	DataType valueType;
	/// Смещение в группе.
	int offset;
	/// Размер массива (1, если не массив).
	int count;

public:
	UniformNode(ptr<UniformGroup> group, DataType valueType, int offset, int count = 1);

	Type GetType() const;

	ptr<UniformGroup> GetGroup() const;
	DataType GetValueType() const;
	int GetOffset() const;
	int GetCount() const;
};

END_INANITY_SHADERS

#endif
