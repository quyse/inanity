#ifndef ___INANITY_DX_SHADER_COMPILER_HPP___
#define ___INANITY_DX_SHADER_COMPILER_HPP___

#include "dx.hpp"
#include "../../String.hpp"

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_GRAPHICS

class ShaderSource;

END_INANITY_GRAPHICS

BEGIN_INANITY_DX

/// Класс компилятора шейдеров из исходников.
/** Данный класс компилирует исходный код шейдера в конкретный шейдер в байт-коде,
принимая список значений параметров.
*/
class ShaderCompiler : public Object
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
	ShaderCompiler();
	/// Создать компилятор с указанными параметрами.
	ShaderCompiler(bool debug, bool optimize, bool columnMajorMatrices = false);

	/// Скомпилировать шейдер.
	ptr<File> Compile(ptr<ShaderSource> shaderSource);
};

END_INANITY_DX

#endif
