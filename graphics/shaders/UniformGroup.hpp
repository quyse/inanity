#ifndef ___INANITY_GRAPHICS_SHADERS_UNIFORM_GROUP_HPP___
#define ___INANITY_GRAPHICS_SHADERS_UNIFORM_GROUP_HPP___

#include "Uniform.hpp"

BEGIN_INANITY

class MemoryFile;

END_INANITY

BEGIN_INANITY_SHADERS

class UniformNode;

/// Группа uniform-переменных.
/** Хранит переменные, и управляет их памятью.
Распределяет переменные, выравнивая их по float4-регистрам. */
class UniformGroup : public Object
{
private:
	/// Номер слота для uniform-буфера.
	int slot;
	/// Требуемый размер буфера.
	int bufferSize;
	/// Память для переменных.
	ptr<MemoryFile> dataFile;
	/// Указатель на данные файла, для производительности.
	void* data;

public:
	UniformGroup(int slot);

	int GetSlot() const;

	/// Добавить узел uniform-переменной.
	template <typename ValueType>
	Uniform<ValueType> AddUniform()
	{
		DataType valueType = GetDataType<ValueType>();
		// получить размер данных переменной
		int valueSize = GetDataTypeSize(valueType);
		// получить смещение до переменной с соответствующим выравниванием
		int offset = (bufferSize + valueSize - 1) / valueSize * valueSize;
		// увеличить размер буфера
		bufferSize = offset + valueSize;

		return Uniform<ValueType>(NEW(UniformNode(this, valueType, offset)));
	}

	/// Финализировать группу.
	/** Выделить память для буфера. После этого добавлять переменные нельзя. */
	void Finalize();

	/// Получить данные группы.
	void* GetData() const;
	/// Получить размер данных группы.
	int GetSize() const;
};

END_INANITY_SHADERS

#endif
