#ifndef ___INANITY_SHADERS_SHADER_HPP___
#define ___INANITY_SHADERS_SHADER_HPP___

#include "DataType.hpp"
#include "Semantic.hpp"
#include "Statement.hpp"
#include <vector>

BEGIN_INANITY_SHADERS

/// Класс шейдера.
class Shader
{
public:
	/// Переменная.
	struct Variable
	{
		/// Тип данных переменной.
		DataType dataType;
		/// Смещение до переменной от начала буфера, в байтах.
		int offset;
		/// Специальная семантика переменной.
		Semantic semantic;

		Variable(DataType dataType, int offset);
	};
	/// Список переменных.
	typedef std::vector<Variable> Variables;

protected:
	/// Весь код шейдера.
	Statement code;

	Variables inputVariables;
	Variables outputVariables;
	Variables tempVariables;
	/// Вектор списков переменных по константным буферам.
	std::vector<Variables> uniformsVariables;
	/// Битовая маска слотов семплеров.
	int samplerSlotsMask;

protected:
	Shader();

	/// Получить переменную по смещению.
	static Variable* GetVariable(Variables& variables, int offset);
	/// Зарегистрировать переменную, если она ещё не зарегистрирована.
	static void RegisterVariable(Variables& variables, DataType dataType, int offset);

	/// Зарегистрировать константный буфер.
	void RegisterUniformBuffer(int slot);

public:
	void SetCode(Statement code);
	Statement GetCode() const;

	const Variables& GetInputVariables() const;
	const Variables& GetOutputVariables() const;
	const Variables& GetTempVariables() const;
	const std::vector<Variables>& GetUniformsVariables() const;
	int GetSamplerSlotsMask() const;
};

END_INANITY_SHADERS

#endif
