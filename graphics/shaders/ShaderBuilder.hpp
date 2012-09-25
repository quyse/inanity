#ifndef ___INANITY_GRAPHICS_SHADERS_SHADER_BUILDER_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SHADER_BUILDER_HPP___

#include "Shader.hpp"
#include "ExpressionObjects.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

/// Константный буфер.
struct UniformBuffer
{
	int slot;
	int offset;
	int size;

	UniformBuffer(int slot, int offset, int size);
};

class ShaderBuilderBase : public Shader
{
protected:
	/// Вектор константных буферов.
	std::vector<UniformBuffer> uniformBuffers;

public:
	/// Добавить константный буфер.
	void RegisterUniformBuffer(int slot, int offset, int size);

	/// Получить семплер.
	Expression sampler(int slot);
};

/// Класс построителя шейдеров.
template <typename Input, typename Output, typename Uniforms, typename Temp>
class ShaderBuilder : public ShaderBuilderBase
{
public:
	typedef Input InputStruct;
	typedef Output OutputStruct;
	typedef Uniforms UniformsStruct;
	typedef Temp TempStruct;

public:
	/// Типизированный метод для добавления константного буфера.
	template <typename BufferType>
	void RegisterUniformBuffer(int slot, BufferType Uniforms::* buffer)
	{
		ShaderBuilderBase::RegisterUniformBuffer(slot, (char*)&(((Uniforms*)0)->*buffer) - (char*)0, sizeof(BufferType));
	}

	/// Получить входную переменную.
	template <typename T>
	Expression in(T Input::* field)
	{
		int offset = (char*)&(((Input*)0)->*field) - (char*)0;
		RegisterVariable(inputVariables, GetDataType<T>(), offset);
		return NEW(InExpressionObject(offset));
	}

	/// Получить выходную переменную.
	template <typename T>
	Expression out(T Output::* field)
	{
		int offset = (char*)&(((Output*)0)->*field) - (char*)0;
		RegisterVariable(outputVariables, GetDataType<T>(), offset);
		return NEW(OutExpressionObject(offset));
	}

	/// Получить uniform-переменную.
	template <typename T, typename BufferType>
	Expression uniform(int bufferSlot, T BufferType::* field)
	{
		int offset = (char*)&(((BufferType*)0)->*field) - (char*)0;
		// зарегистрировать переменную в буфере
		RegisterVariable(uniformsVariables[bufferSlot], GetDataType<T>(), offset);
		// вернуть её
		return NEW(UniformExpressionObject(uniformBuffers[bufferSlot].slot, offset));
	}

	/// Получить временную переменную.
	template <typename T>
	Expression temp(T Temp::* field)
	{
		int offset = (char*)&(((Temp*)0)->*field) - (char*)0;
		RegisterVariable(tempVariables, GetDataType<T>(), offset);
		return NEW(TempExpressionObject(offset));
	}

	/// Установить входную семантику.
	template <typename T>
	void semantic(T Input::* field, Semantic semantic)
	{
		int offset = (char*)&(((Input*)0)->*field) - (char*)0;
		Variable* variable = GetVariable(inputVariables, offset);
		if(!variable)
			THROW_PRIMARY_EXCEPTION("Can't find variable to set input semantic");
		variable->semantic = semantic;
	}

	/// Установить выходную семантику.
	template <typename T>
	void semantic(T Output::* field, Semantic semantic)
	{
		int offset = (char*)&(((Output*)0)->*field) - (char*)0;
		Variable* variable = GetVariable(outputVariables, offset);
		if(!variable)
			THROW_PRIMARY_EXCEPTION("Can't find variable to set input semantic");
		variable->semantic = semantic;
	}
};

END_INANITY_SHADERS

#endif
