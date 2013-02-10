#include "PngImageLoader.hpp"
#include "Image2DData.hpp"
#include "../deps/libpng/png.hpp"
#include "../MemoryFile.hpp"
#include "../Exception.hpp"
#include <sstream>

class PngImageLoader::Helper
{
public:
	struct IO
	{
		const char* data;
		size_t size;
		size_t position;
	};

	static void Error(png_structp png, png_const_charp errorMsg);
	static void Warning(png_structp png, png_const_charp warningMsg);
	static void Read(png_structp png, png_bytep data, png_size_t length);
};

void PngImageLoader::Helper::Error(png_structp png, png_const_charp errorMsg)
{
	std::ostringstream& errorStream = *(std::ostringstream*)png_get_error_ptr(png);
	errorStream << "ERROR: " << errorMsg;
}

void PngImageLoader::Helper::Warning(png_structp png, png_const_charp warningMsg)
{
	std::ostringstream& errorStream = *(std::ostringstream*)png_get_error_ptr(png);
	errorStream << "WARNING: " << warningMsg;
}

void PngImageLoader::Helper::Read(png_structp png, png_bytep data, png_size_t length)
{
	IO& io = *(IO*)png_get_io_ptr(png);
	// проверить, что данных хватает
	if(io.position + length > io.size)
	{
		png_error(png, "Not enough data to read");
		return;
	}
	// скопировать данные
	memcpy(data, io.data + io.position, length);
	// сдвинуть счётчик
	io.position += length;
}

ptr<Image2DData> PngImageLoader::Load(ptr<File> file)
{
	try
	{
		// получить данные
		char* fileData = (char*)file->GetData();
		size_t fileSize = file->GetSize();

		// проверить сигнатуру
		if(fileSize < 8 || png_sig_cmp((png_const_bytep)fileData, 0, 8) != 0)
			THROW_PRIMARY_EXCEPTION("PNG signature check failed");

		/* Обработка ошибок в libpng идёт через setjmp/longjmp. Сначала устанавливается
		точка возврата вызовом setjmp, затем выполняются необходимые вызовы функций libpng.
		Если будет ошибка, libpng вернёт управление в точку возврата setjmp, и мы обработаем
		ошибку. Исключения вызывать нельзя, потому что тогда мы пропустим очистку. Объекты C++
		создавать тоже нельзя, потому что их удаление будет пропущено при longjmp.
		*/

		// поток для сообщений об ошибках
		std::ostringstream errorStream;

		// структура ввода-вывода
		Helper::IO io;
		io.data = fileData;
		io.size = fileSize;
		io.position = 8;

		// память для изображения
		ptr<MemoryFile> imageFile;
		png_bytep* imageRows = 0;

		// создать объекты libpng
		png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, &errorStream, Helper::Error, Helper::Warning);
		if(!pngPtr)
			THROW_PRIMARY_EXCEPTION("Can't create read struct");
		png_infop infoPtr = png_create_info_struct(pngPtr);
		if(!infoPtr)
		{
			png_destroy_read_struct(&pngPtr, (png_infopp)NULL, (png_infopp)NULL);
			free(imageRows);
			THROW_PRIMARY_EXCEPTION("Can't create info struct");
		}

		// блок обработки ошибок
		// объявляется режим тревоги, далее на C++ писать с осторожностью
		if(setjmp(png_jmpbuf(pngPtr)))
		{
			// если управление здесь, произошла ошибка
			// удалить структуру
			png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)NULL);
			// выбросить исключение (режим тревоги уже кончился)
			THROW_PRIMARY_EXCEPTION("Error when reading PNG: " + errorStream.str());
		}

		// установить функцию для чтения данных
		png_set_read_fn(pngPtr, &io, &Helper::Read);

		// установить количество уже прочитанных байт
		png_set_sig_bytes(pngPtr, 8);

		// считать информацию об изображении
		png_read_info(pngPtr, infoPtr);
		int width = png_get_image_width(pngPtr, infoPtr);
		int height = png_get_image_height(pngPtr, infoPtr);
		int bitDepth = png_get_bit_depth(pngPtr, infoPtr);
		int colorType = png_get_color_type(pngPtr, infoPtr);

		//** установить настройки преобразования
		// если изображение палитровое, преобразовать в RGB
		if(colorType == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(pngPtr);
		// если изображение в оттенках серого, и меньше 8 бит на пиксел, сделать 8 бит
		if(colorType == PNG_COLOR_TYPE_GRAY && bitDepth < 8)
			png_set_expand_gray_1_2_4_to_8(pngPtr);
		// установить режим обработки прозрачности
		png_set_alpha_mode(pngPtr, PNG_ALPHA_PNG, PNG_DEFAULT_sRGB);

		// обновить информацию об изображении, чтобы вычислился правильный размер
		// строки после преобразований
		png_read_update_info(pngPtr, infoPtr);
		// вот этот размер
		int pitch = png_get_rowbytes(pngPtr, infoPtr);
		// выделить память под изображение
		// в отдельном try-catch, чтобы исключение не выпало
		try
		{
			imageFile = NEW(MemoryFile(height * pitch));
		}
		catch(Exception* exception)
		{
			// бросаем "исключение" libpng
			png_error(pngPtr, "Can't allocate image memory");
		}
		imageRows = (png_bytep*)malloc(height * sizeof(void*));
		png_bytep imageFileData = (png_bytep)imageFile->GetData();
		for(int i = 0; i < height; ++i)
			imageRows[i] = imageFileData + i * pitch;

		// считать изображение
		png_read_image(pngPtr, imageRows);

		// завершить считывание
		// по идее, это только ставит курсор на завершение PNG-файла
		// так что не делаем
		//png_read_end(pngPtr, (png_infop)NULL);

		// очистить память
		png_destroy_read_struct(&pngPtr, &infoPtr, (png_infopp)NULL);
		free(imageRows);

		// всё очищено, режим тревоги отменяется, теперь можно писать на C++ снова ^_^
		// создать и вернуть объект данных изображения
		return NEW(Image2DData(imageFile, width, height, pitch, PixelFormats::intR8G8B8A8));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't load PNG image", exception);
	}
}
