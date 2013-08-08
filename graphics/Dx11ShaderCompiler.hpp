#ifndef ___INANITY_GRAPHICS_DX11_SHADER_COMPILER_HPP___
#define ___INANITY_GRAPHICS_DX11_SHADER_COMPILER_HPP___

#include "ShaderCompiler.hpp"
#include "../FileSystem.hpp"
#include "../platform/DllFunction.hpp"
#include "d3d11.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс компилятора шейдеров из исходников.
/** Данный класс компилирует исходный код шейдера в конкретный шейдер в байт-коде,
принимая список значений параметров.
*/
class Dx11ShaderCompiler : public ShaderCompiler
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

	Platform::DllFunction<pD3DCompile> functionD3DCompile;

	class IncludeProcessor;

public:
	Dx11ShaderCompiler(
#ifdef _DEBUG
		bool debug = true,
		bool optimize = false,
#else
		bool debug = false,
		bool optimize = true,
#endif
		bool columnMajorMatrices = true,
		ptr<FileSystem> includesFileSystem = 0
	);

	ptr<File> Compile(ptr<ShaderSource> shaderSource);
};

END_INANITY_GRAPHICS

#endif
