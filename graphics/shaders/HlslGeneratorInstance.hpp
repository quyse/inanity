#ifndef ___INANITY_GRAPHICS_SHADERS_HLSL_GENERATOR_INSTANCE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_HLSL_GENERATOR_INSTANCE_HPP___

#include "Shader.hpp"
#include "Statement.hpp"
#include <sstream>

BEGIN_INANITY_SHADERS

/// Экземпляр генератора шейдеров на HLSL.
class HlslGeneratorInstance
{
private:
	/// Исходный шейдер.
	const Shader& shader;
	/// Результирующий текст шейдера на HLSL.
	std::ostringstream hlsl;
	/// Текущее количество табов в начале строки.
	int tabsCount;

private:
	/// Напечатать отступ нужной длины.
	void PrintTabs();
	/// Напечатать имя типа.
	void PrintDataType(DataType dataType);
	/// Напечатать структуру с объявлениями.
	void PrintVariables(Shader::Variables variables, const char* variableNamePrefix, bool printPackOffsets);
	/// Напечатать statement.
	void PrintStatement(Statement statement);
	/// Напечатать выражение.
	void PrintExpression(Expression expression);
	/// Получить имя функции HLSL, если оно преобразовывается.
	static const char* GetRealFunctionName(const char* functionName);

public:
	HlslGeneratorInstance(const Shader& shader);

	std::string Generate();
};

END_INANITY_SHADERS

#endif
