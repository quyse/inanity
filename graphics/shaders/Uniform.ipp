#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_IPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_IPP___

#include "Uniform.hpp"
#include "UniformNode.hpp"
#include "UniformGroup.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Uniform<ValueType>::Uniform(ptr<UniformNode> node)
: Value<ValueType>(ptr<Node>(node))
{
#ifdef _DEBUG
	if(node->GetValueType() != DataTypeOf<ValueType>())
		THROW("Wrong uniform node type");
#endif
}

template <typename ValueType>
void Uniform<ValueType>::SetValue(const ValueType& value)
{
	UniformNode* uniformNode = fast_cast<UniformNode*>(&*this->node);
	*(ValueType*)((char*)uniformNode->GetGroup()->GetData() + uniformNode->GetOffset()) = value;
}

END_INANITY_SHADERS

#endif
