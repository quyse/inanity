#include "Dx11ShaderCompiler.hpp"
#include "Hlsl11Source.hpp"
#include "Dx11CompiledShader.hpp"
#include "D3DBlobFile.hpp"
#include "../FileSystem.hpp"
#include "../File.hpp"
#include "../MemoryStream.hpp"
#include "../Strings.hpp"
#include "../platform/DllFunction.ipp"
#include "../Exception.hpp"

BEGIN_INANITY_GRAPHICS

/// Класс обработчика включаемых файлов.
/** Предоставляет интерфейс ID3DInclude, необходимый для компиляции шейдеров,
возвращая файлы из заданной файловой системы. */
class Dx11ShaderCompiler::IncludeProcessor : public ID3DInclude
{
private:
	/// Файловая система.
	ptr<FileSystem> fileSystem;
	/// Список открытых в данный момент файлов
	/** Реализован в виде тупо массива, потому что шейдеры редко будут использовать
	уйму включаемых файлов, и так будет быстрее. */
	std::vector<ptr<File> > files;

public:
	IncludeProcessor(ptr<FileSystem> fileSystem) : fileSystem(fileSystem) {}

	/// Открыть файл.
	HRESULT STDMETHODCALLTYPE Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
	{
		// если файл не локальный, закончить (потому что системных файлов пока не обрабатываем)
		if(IncludeType != D3D_INCLUDE_LOCAL)
			return E_FAIL;

		// получить файл из файловой системы
		ptr<File> file = fileSystem->TryLoadFile(pFileName);
		if(!file)
			return E_FAIL;

		// добавить его в список открытых
		files.push_back(file);
		
		// вернуть указатель на данные файла, и размер файла
		*ppData = file->GetData();
		*pBytes = (UINT)file->GetSize();

		return S_OK;
	}

	/// Закрыть файл.
	HRESULT STDMETHODCALLTYPE Close(LPCVOID pData)
	{
		// найти и закрыть файл
		for(size_t i = 0; i < files.size(); ++i)
			if(files[i]->GetData() == pData)
			{
				files.erase(files.begin() + i);
				return S_OK;
			}
		// если управление здесь, значит, нас просят закрыть непонятный файл
		return E_FAIL;
	}
};

Dx11ShaderCompiler::Dx11ShaderCompiler(
	bool debug,
	bool optimize,
	bool columnMajorMatrices,
	ptr<FileSystem> includesFileSystem) :
	debug(debug), optimize(optimize), columnMajorMatrices(columnMajorMatrices),
	includesFileSystem(includesFileSystem),
	functionD3DCompile(D3DCOMPILER_DLL_A, "D3DCompile")
{
}

ptr<File> Dx11ShaderCompiler::Compile(ptr<ShaderSource> shaderSource)
{
	try
	{
		// получить код шейдера на HLSL
		ptr<Hlsl11Source> hlslSource = shaderSource.FastCast<Hlsl11Source>();
		if(!hlslSource)
			THROW("Shader source is not HLSL");

		// получить код шейдера
		ptr<File> code = hlslSource->GetCode();

		// обработчик включаемых файлов
		IncludeProcessor includeProcessor(includesFileSystem);

		// скомпилировать шейдер
		ID3DBlob* shaderBlob;
		ID3DBlob* errorsBlob;
		HRESULT result = functionD3DCompile(
			(char*)code->GetData(),
			code->GetSize(),
			NULL, // source name
			NULL, // defines
			includesFileSystem ? &includeProcessor : 0, // includes
			hlslSource->GetFunctionName().c_str(), // entrypoint
			hlslSource->GetProfile().c_str(), // target
			D3DCOMPILE_ENABLE_STRICTNESS
			| (debug ? D3DCOMPILE_DEBUG : 0)
			| (optimize ? D3DCOMPILE_OPTIMIZATION_LEVEL3 : (D3DCOMPILE_OPTIMIZATION_LEVEL0 | D3DCOMPILE_SKIP_OPTIMIZATION))
			| (columnMajorMatrices ? D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR : D3DCOMPILE_PACK_MATRIX_ROW_MAJOR)
			, // flags
			0, // flags 2
			&shaderBlob,
			&errorsBlob
		);

		// завернуть blob'ы в классы для корректного удаления
		ptr<D3DBlobFile> shaderFile = NEW(D3DBlobFile(shaderBlob));
		ptr<D3DBlobFile> errorsFile = NEW(D3DBlobFile(errorsBlob));

		// если ошибка
		if(FAILED(result))
			THROW("Compile error(s): " + Strings::File2String(errorsFile) + "\n" + Strings::File2String(code));

		// создать скомпилированный шейдер
		ptr<Dx11CompiledShader> compiledShader = NEW(Dx11CompiledShader(shaderFile, hlslSource->GetResources()));
		// сериализовать его в файл и вернуть
		ptr<MemoryStream> memoryStream = NEW(MemoryStream());
		compiledShader->Serialize(memoryStream);
		return memoryStream->ToFile();
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY("Can't compile shader", exception);
	}
}

END_INANITY_GRAPHICS
