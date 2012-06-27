#include "Shader.hpp"

std::string Shader::getSource() const
{
	std::ostringstream source;

#ifdef ___INANITY_SHADERS_HLSL
	// входная структура
	source << "struct Input\n{\n" << inputDeclarations.str() << "};\n";
	// выходная структура
	source << "struct Output\n{\n" << outputDeclarations.str() << "};\n";
	// глобальные переменные
	source << globalDeclarations;
	// начало функции
	source << "Output f(Input input)\n{Output output;\n";
	// временные переменные
	source << tempDeclarations.str();
	// действия
	source << statements.str();
	// конец функции
	source << "return output;\n}\n";
#endif

#ifdef ___INANITY_SHADERS_GLSL
	// атрибуты
	source << inputDeclarations;
	source << globalDeclarations;
#endif
}
