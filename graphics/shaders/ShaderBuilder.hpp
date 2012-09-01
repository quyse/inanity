#ifndef ___INANITY_GRAPHICS_SHADERS_SHADER_BUILDER_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SHADER_BUILDER_HPP___

#include "Shader.hpp"
#include "ExpressionObjects.hpp"

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
private:
	/// Вектор константных буферов.
	std::vector<UniformBuffer> uniformBuffers;

public:
	/// Добавить константный буфер.
	void uniformBuffer(int slot, int offset, int size);

	/// Получить семплер.
	Expression sampler(int slot);
};

/// Класс построителя шейдеров.
template <typename Input, typename Output, typename Uniforms, typename Temp>
class ShaderBuilder
{
public:
	typedef Input InputStruct;
	typedef Output OutputStruct;
	typedef Uniforms UniformsStruct;
	typedef Temp TempStruct;

public:
	/// Типизированный метод для добавления константного буфера.
	template <typename BufferType>
	void uniformBuffer(int slot, BufferType Uniforms::* buffer)
	{
		uniformBuffer(slot, (char*)(((Uniforms*)0)->*buffer) - (char*)0, sizeof(BufferType));
	}

	/// Получить входную переменную.
	template <typename T>
	Expression in(T Input::* field)
	{
		int offset = (char*)(((Input*)0)->*field) - (char*)0;
		RegisterVariable(inputVariables, GetDataType<T>(), offset);
		return NEW(InExpressionObject(offset));
	}

	/// Получить выходную переменную.
	template <typename T>
	Expression out(T Output::* field)
	{
		int offset = (char*)(((Output*)0)->*field) - (char*)0;
		RegisterVariable(outputVariables, GetDataType<T>(), offset);
		return NEW(OutExpressionObject(offset));
	}

	/// Получить uniform-переменную.
	template <typename T>
	Expression uniform(T Uniforms::* field)
	{
		// получить глобальное смещение
		int offset = (char*)(((Uniforms*)0)->*field) - (char*)0;
		// получить, в каком буфере находится переменная.
		for(size_t i = 0; i < uniformBuffers.size(); ++i)
			if(offset >= uniformBuffers[i].offset && offset < uniformBuffers[i].offset + uniformBuffers[i].size)
			{
				// вычислить локальное смещение
				offset -= uniformBuffers[i].offset;
				// зарегистрировать переменную в буфере
				RegisterVariable(uniformsVariables[i], GetDataType<T>(), offset);
				// вернуть её
				return NEW(UniformExpressionObject(uniformBuffers[i].slot, offset));
			}
		// буфер не найден
		THROW_PRIMARY_EXCEPTION("Uniform buffer holding this offset is not found");
	}

	/// Получить временную переменную.
	template <typename T>
	Expression temp(T Temp::* field)
	{
		int offset = (char*)(((Temp*)0)->*field) - (char*)0;
		RegisterVariable(tempVariables, GetDataType<T>(), offset);
		return NEW(TempExpressionObject(offset));
	}
};

END_INANITY_SHADERS

#endif
