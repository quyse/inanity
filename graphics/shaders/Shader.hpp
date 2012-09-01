#ifndef ___INANITY_SHADERS_SHADER_HPP___
#define ___INANITY_SHADERS_SHADER_HPP___

#include "DataType.hpp"
#include "Statement.hpp"
#include <vector>

BEGIN_INANITY_SHADERS

/// Класс шейдера.
class Shader
{
protected:
	Statement code;

	/// Переменная.
	struct Variable
	{
		DataType dataType;
		int offset;

		Variable(DataType dataType, int offset);
	};
	/// Список переменных.
	typedef std::vector<Variable> Variables;

	Variables inputVariables;
	Variables outputVariables;
	Variables tempVariables;
	/// Вектор списков переменных по константным буферам.
	std::vector<Variables> uniformsVariables;
	/// Битовая маска слотов семплеров.
	int samplerSlotsMask;

protected:
	Shader();

	/// Зарегистрировать переменную, если она ещё не зарегистрирована.
	static void RegisterVariable(Variables& variables, DataType dataType, int offset);

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
