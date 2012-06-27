#ifndef ___INANITY_DX_SHADER_DATA_HPP___
#define ___INANITY_DX_SHADER_DATA_HPP___

#include "MultiShaderType.h"
#include "MultiShaderConstantNode.h"
#include "GraphicsMath.h"
#include "Texture.h"
#include "SamplerState.h"
#include "Exception.h"
#include <vector>

BEGIN_INANITY_DX

/// Класс данных для шейдера.
/** Предназначен для указания данных, которые имеются у заказчика; эти данные затем могут быть использованы
в шейдере.
Конкретно, класс позволяет указывать произвольные данные, передаваемые в константном буфере (глобальные переменные),
а также текстуры и семплеры.
*/
class ShaderData : public Object
{
public:
	struct Constant
	{
		unsigned offset;
		MultiShaderType type;

		Constant(unsigned offset, const MultiShaderType& type);
	};

private:
	std::vector<Constant> constants;

public:
	const std::vector<Constant>& GetConstants() const;

	ptr<MultiShaderConstantNode> AddConstant(size_t offset, const MultiShaderType& type);

	template <typename T>
	class Binder
	{
	private:
		const T* object;
		ptr<ShaderData> shaderData;

	public:
		template <typename Field>
		size_t ConvertToOffset(const Field& field)
		{
			return (size_t)((const char*)&field - (const char*)object);
		}

	private:
		template <typename Field>
		MultiShaderType GetType()
		{
			THROW_PRIMARY_EXCEPTION(L"Unknown constant type");
		}
		template <>
		MultiShaderType GetType<float>()
		{
			return MultiShaderType(MultiShaderType::elementTypeFloat);
		}
		template <>
		MultiShaderType GetType<float2>()
		{
			return MultiShaderType(MultiShaderType::elementTypeFloat, MultiShaderType::vectorTypeVector2);
		}
		template <>
		MultiShaderType GetType<float3>()
		{
			return MultiShaderType(MultiShaderType::elementTypeFloat, MultiShaderType::vectorTypeVector3);
		}
		template <>
		MultiShaderType GetType<float4>()
		{
			return MultiShaderType(MultiShaderType::elementTypeFloat, MultiShaderType::vectorTypeVector4);
		}
		template <>
		MultiShaderType GetType<float4x4>()
		{
			return MultiShaderType(MultiShaderType::elementTypeFloat, MultiShaderType::vectorTypeMatrix4x4);
		}
		template <>
		MultiShaderType GetType<ptr<Texture> >()
		{
			return MultiShaderType(MultiShaderType::objectTypeTexture2D);
		}
		template <>
		MultiShaderType GetType<ptr<SamplerState> >()
		{
			return MultiShaderType(MultiShaderType::objectTypeSampler);
		}

	public:
		Binder(const T* object, ptr<ShaderData> shaderData) : object(object), shaderData(shaderData) {}

		template <typename Field>
		ptr<MultiShaderConstantNode> AddConstant(const Field& field)
		{
			return shaderData->AddConstant(ConvertToOffset(field), GetType<Field>());
		}

		template <typename ElementType, int n>
		ptr<MultiShaderConstantNode> AddConstantArray(const ElementType (&fields)[n])
		{
			MultiShaderType type = GetType<ElementType>();
			return shaderData->AddConstant(ConvertToOffset(fields), MultiShaderType(type.GetElementType(), type.GetVectorType(), n, type.GetObjectType()));
		}
	};

	template <typename T>
	Binder<T> GetBinder(const T* object)
	{
		return Binder<T>(object, this);
	}
};

END_INANITY_DX

#endif
