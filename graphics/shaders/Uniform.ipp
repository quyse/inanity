#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_IPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_IPP___

#include "Uniform.hpp"
#include "UniformNode.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

template <typename ValueType>
Uniform<ValueType>::Uniform(ptr<UniformNode> node)
: Value<ValueType>(node)
{
#ifdef _DEBUG
	if(node->GetValueType() != GetDataType<ValueType>())
		THROW_PRIMARY_EXCEPTION("Wrong uniform node type");
#endif
}

/// Скопировать данные переменной.
template <typename ValueType>
inline void CopyUniformData(ValueType& data, const ValueType& v)
{
	data = v;
}
// Для матриц требуется транспонирование.
template <>
inline void CopyUniformData(float4x4& data, const float4x4& v)
{
	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 4; ++j)
			data.t[i][j] = v.t[j][i];
}

template <typename ValueType>
void Uniform<ValueType>::SetValue(const ValueType& value)
{
	UniformNode* uniformNode = fast_cast<UniformNode*>(&*node);
	CopyUniformData<ValueType>(*(ValueType*)((char*)uniformNode->GetGroup()->GetData() + uniformNode->GetOffset()), value);
}

END_INANITY_SHADERS

#endif
