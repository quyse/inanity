#include "DxShaderCompiler.hpp"
#include "ShaderSource.hpp"
#include "D3D10BlobFile.hpp"
#include "../FileSystem.hpp"
#include "../File.hpp"
#include "../Strings.hpp"
#include "../Exception.hpp"
#include "d3dx.hpp"

/// Класс обработчика включаемых файлов.
/** Предоставляет интерфейс ID3D10Include, необходимый для компиляции шейдеров,
возвращая файлы из заданной файловой системы. */
class IncludeProcessor : public Object, public ID3D10Include
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
	HRESULT STDMETHODCALLTYPE Open(D3D10_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes)
	{
		//если файл не локальный, закончить (потому что системных файлов пока не обрабатываем)
		if(IncludeType != D3D10_INCLUDE_LOCAL)
			return E_FAIL;

		//получить файл из файловой системы
		ptr<File> file = fileSystem->TryLoadFile(pFileName);
		if(!file)
			return E_FAIL;

		//добавить его в список открытых
		files.push_back(file);
		
		//вернуть указатель на данные файла, и размер файла
		*ppData = file->GetData();
		*pBytes = (UINT)file->GetSize();

		return S_OK;
	}

	/// Закрыть файл.
	HRESULT STDMETHODCALLTYPE Close(LPCVOID pData)
	{
		//найти и закрыть файл
		for(size_t i = 0; i < files.size(); ++i)
			if(files[i]->GetData() == pData)
			{
				files.erase(files.begin() + i);
				return S_OK;
			}
		//если управление здесь, значит, нас просят закрыть непонятный файл
		return E_FAIL;
	}
};

DxShaderCompiler::DxShaderCompiler()
	:
#ifdef _DEBUG
	debug(true), optimize(false), columnMajorMatrices(true)
#else
	debug(false), optimize(true), columnMajorMatrices(true)
#endif
{
}

DxShaderCompiler::DxShaderCompiler(bool debug, bool optimize, bool columnMajorMatrices)
	: debug(debug), optimize(optimize), columnMajorMatrices(columnMajorMatrices)
{
}

ptr<File> DxShaderCompiler::Compile(ptr<ShaderSource> shaderSource)
{
	try
	{
		//получить код шейдера
		ptr<File> code = shaderSource->GetCode();

		//создать обработчик включаемых файлов, если нужно
		ptr<FileSystem> includesFileSystem = shaderSource->GetIncludes();
		ptr<IncludeProcessor> includeProcessor;
		if(includesFileSystem)
			includeProcessor = NEW(IncludeProcessor(includesFileSystem));

		//скомпилировать шейдер
		ID3D10Blob* shaderBlob;
		ID3D10Blob* errorsBlob;
		HRESULT result = D3DX11CompileFromMemory((char*)code->GetData(), code->GetSize(), NULL, NULL,
			includeProcessor ? includeProcessor : 0, shaderSource->GetFunctionName().c_str(),
			shaderSource->GetProfile().c_str(), D3D10_SHADER_ENABLE_STRICTNESS
			| (debug ? D3D10_SHADER_DEBUG : 0)
			| (optimize ? D3D10_SHADER_OPTIMIZATION_LEVEL3 : (D3D10_SHADER_OPTIMIZATION_LEVEL0 | D3D10_SHADER_SKIP_OPTIMIZATION))
			| (columnMajorMatrices ? D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR : D3D10_SHADER_PACK_MATRIX_ROW_MAJOR)
			, 0, NULL, &shaderBlob, &errorsBlob, NULL);

		//завернуть blob'ы в классы для корректного удаления
		ptr<D3D10BlobFile> shaderFile = NEW(D3D10BlobFile(shaderBlob));
		ptr<D3D10BlobFile> errorsFile = NEW(D3D10BlobFile(errorsBlob));

		//если ошибка
		if(FAILED(result))
			THROW_PRIMARY_EXCEPTION("Compile error(s): " + Strings::File2String(errorsFile));

		//вернуть файл с шейдером
		return shaderFile;
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't compile shader", exception);
	}
}
