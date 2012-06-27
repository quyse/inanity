#ifndef ___INANITY_SHADER_HPP___
#define ___INANITY_SHADER_HPP___

#include "dx.hpp"

BEGIN_INANITY_DX

/// Класс скомпилированного шейдера.
/** Хранит объект рефлексии, который используется для создания
входных разметок. От данного класса наследуются конкретные классы
вершинного, пиксельного и других шейдеров. */
class ShaderReflection;
class Shader : public Object
{
private:
	ptr<ShaderReflection> shaderReflection;

public:
	/// Создать шейдер.
	Shader(ptr<ShaderReflection> shaderReflection);

	/// Получить объект рефлексии
	ptr<ShaderReflection> GetReflection() const;
};

END_INANITY_DX

#endif
