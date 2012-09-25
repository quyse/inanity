#ifndef ___INANITY_SHADERS_SHADER_HPP___
#define ___INANITY_SHADERS_SHADER_HPP___

#include "DataType.hpp"
#include "SamplerType.hpp"
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

	/// Семплер.
	struct Sampler
	{
		/// Тип семплера.
		SamplerType samplerType;
		/// Тип семплов.
		DataType dataType;

		Sampler(SamplerType samplerType = SamplerTypes::_None, DataType dataType = DataTypes::Float4);
	};

protected:
	/// Весь код шейдера.
	Statement code;

	Variables inputVariables;
	Variables outputVariables;
	Variables tempVariables;
	/// Вектор списков переменных по константным буферам.
	std::vector<Variables> uniformsVariables;
	/// Вектор семплеров.
	std::vector<Sampler> samplers;

protected:
	/// Получить переменную по смещению.
	static Variable* GetVariable(Variables& variables, int offset);
	/// Зарегистрировать переменную, если она ещё не зарегистрирована.
	static void RegisterVariable(Variables& variables, DataType dataType, int offset);

public:
	/// Зарегистрировать константный буфер.
	void RegisterUniformBuffer(int slot);
	/// Зарегистрировать семплер.
	void RegisterSampler(int slot, SamplerType samplerType, DataType dataType);

	void SetCode(Statement code);
	Statement GetCode() const;

	const Variables& GetInputVariables() const;
	const Variables& GetOutputVariables() const;
	const Variables& GetTempVariables() const;
	const std::vector<Variables>& GetUniformsVariables() const;
	const std::vector<Sampler> GetSamplers() const;
};

END_INANITY_SHADERS

#endif
