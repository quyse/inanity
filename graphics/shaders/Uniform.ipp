#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_IPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_IPP___

#include "Uniform.hpp"
#include "UniformNode.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Uniform<ValueType>::Uniform(ptr<UniformNode> node)
: LValue(node)
{
#ifdef _DEBUG
	if(node->GetValueType() != GetDataType<ValueType>())
		THROW_PRIMARY_EXCEPTION("Wrong uniform node type");
#endif
}

template <typename ValueType>
void Uniform<ValueType>::SetValue(ValueType value)
{
	UniformNode* uniformNode = fast_cast<UniformNode*>(&*node);
	*(ValueType*)((char*)uniformNode->GetBuffer()->GetData() + uniformNode->GetOffset()) = value;
}

END_INANITY_SHADERS

#endif
