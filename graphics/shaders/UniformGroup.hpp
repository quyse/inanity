#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_GROUP_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_GROUP_HPP___

#include "Uniform.hpp"
#include "UniformArray.hpp"
#include "UniformNode.hpp"
#include "Value.ipp"
#include "../DataType.hpp"

BEGIN_INANITY

class MemoryFile;

END_INANITY

BEGIN_INANITY_GRAPHICS

class Device;
class UniformBuffer;
class Context;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

class UniformNode;

/// Group of uniform variables.
class UniformGroup : public Object
{
private:
	/// Slot number of uniform buffer.
	int slot;
	/// Uniform-буфер.
	ptr<UniformBuffer> buffer;
	/// Size of buffer.
	int bufferSize;
	/// Memory for variables.
	ptr<MemoryFile> dataFile;
	/// Data pointer (for performance).
	void* data;

public:
	UniformGroup(int slot);

	int GetSlot() const;
	ptr<UniformBuffer> GetBuffer() const;

	/// Добавить узел uniform-переменной.
	template <typename ValueType>
	Uniform<ValueType> AddUniform()
	{
		DataType valueType = DataTypeOf<ValueType>();
		// получить размер данных переменной
		int valueSize = GetDataTypeSize(valueType);
		// получить смещение до переменной с соответствующим выравниванием
		// на границу vec4-регистра
		int offset = bufferSize;
		if(offset % sizeof(vec4) + valueSize > sizeof(vec4))
			offset = (offset + sizeof(vec4) - 1) & ~(sizeof(vec4) - 1);
		// увеличить размер буфера
		bufferSize = offset + valueSize;

		return Uniform<ValueType>(NEW(UniformNode(this, valueType, offset)));
	}

	/// Добавить узел uniform-массива.
	template <typename ValueType>
	UniformArray<ValueType> AddUniformArray(int count)
	{
		DataType valueType = DataTypeOf<ValueType>();
		// получить размер данных переменной
		int valueSize = GetDataTypeSize(valueType) * count;
		// получить смещение до переменной с соответствующим выравниванием
		// на границу vec4-регистра
		int offset = bufferSize;
		if(offset % sizeof(vec4) + valueSize > sizeof(vec4))
			offset = (offset + sizeof(vec4) - 1) & ~(sizeof(vec4) - 1);
		// увеличить размер буфера
		bufferSize = offset + valueSize;

		return UniformArray<ValueType>(NEW(UniformNode(this, valueType, offset, count)));
	}

	/// Финализировать группу.
	/** Выделить память для буфера. После этого добавлять переменные нельзя. */
	void Finalize(ptr<Device> device);

	/// Получить данные группы.
	void* GetData() const;
	/// Получить размер данных группы.
	int GetSize() const;
	/// Загрузить данные буфера в контекст.
	void Upload(Context* context);
};

END_INANITY_SHADERS

#endif
