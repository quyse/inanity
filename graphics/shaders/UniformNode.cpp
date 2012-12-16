#include "UniformNode.hpp"

UniformNode::UniformNode(ptr<UniformBuffer> buffer, int offset, DataType valueType)
: buffer(buffer), offset(offset), valueType(valueType) {}

ptr<UniformBuffer> UniformNode::GetBuffer() const
{
	return buffer;
}

int UniformNode::GetOffset() const
{
	return offset;
}

DataType UniformNode::GetValueType() const
{
	return valueType;
}
