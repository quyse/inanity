#ifndef ___INANITY_SHADERS_SHADER_HPP___
#define ___INANITY_SHADERS_SHADER_HPP___

#include "shaders.hpp"
#include <sstream>
#include <string>

BEGIN_INANITY_SHADERS

/// Класс, обозначающий строящийся шейдер.
class Shader
{
public:
	/// Текст объявлений глобальных переменных.
	std::ostringstream globalDeclarations;
	/// Текст объявлений временных переменных.
	std::ostringstream tempDeclarations;
	/// Текст объявлений входных данных.
	std::ostringstream inputDeclarations;
	/// Текст объявлений выходных данных.
	std::ostringstream outputDeclarations;
	/// Текст собственно действий.
	std::ostringstream statements;

public:
	/// Получить результирующий текст.
	std::string getSource() const;
};

END_INANITY_SHADER

#endif
