#ifndef ___INANITY_GRAPHICS_DX_SHADER_COMPILER_HPP___
#define ___INANITY_GRAPHICS_DX_SHADER_COMPILER_HPP___

#include "ShaderCompiler.hpp"

BEGIN_INANITY

class FileSystem;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс компилятора шейдеров из исходников.
/** Данный класс компилирует исходный код шейдера в конкретный шейдер в байт-коде,
принимая список значений параметров.
*/
class DxShaderCompiler : public ShaderCompiler
{
private:
	/// Включить возможность отладки шейдеров.
	bool debug;
	/// Включить оптимизацию.
	bool optimize;
	/// Использовать column-major матрицы.
	bool columnMajorMatrices;
	/// Файловая система для включаемых файлов (опционально).
	ptr<FileSystem> includesFileSystem;

	class IncludeProcessor;

public:
	/// Создать компилятор с параметрами по умолчанию.
	DxShaderCompiler();
	/// Создать компилятор с указанными параметрами.
	DxShaderCompiler(bool debug, bool optimize, bool columnMajorMatrices, ptr<FileSystem> includesFileSystem);

	ptr<File> Compile(ptr<ShaderSource> shaderSource);
};

END_INANITY_GRAPHICS

#endif
