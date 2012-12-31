#ifndef ___INANITY_GRAPHICS_DX_SHADER_COMPILER_HPP___
#define ___INANITY_GRAPHICS_DX_SHADER_COMPILER_HPP___

#include "graphics.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

class ShaderSource;

/// Класс компилятора шейдеров из исходников.
/** Данный класс компилирует исходный код шейдера в конкретный шейдер в байт-коде,
принимая список значений параметров.
*/
class DxShaderCompiler : public Object
{
private:
	/// Включить возможность отладки шейдеров.
	bool debug;
	/// Включить оптимизацию.
	bool optimize;
	/// Использовать column-major матрицы.
	bool columnMajorMatrices;

public:
	/// Создать компилятор с параметрами по умолчанию.
	DxShaderCompiler();
	/// Создать компилятор с указанными параметрами.
	DxShaderCompiler(bool debug, bool optimize, bool columnMajorMatrices = true);

	/// Скомпилировать шейдер.
	ptr<File> Compile(ptr<ShaderSource> shaderSource);
};

END_INANITY_GRAPHICS

#endif
