#include "SystemFontCreator.hpp"
#include <iostream>
#include <unordered_set>
#include <windowsx.h>

int min2(int n)
{
	int i;
	for(i = 1; i < n; i <<= 1);
	return i;
}

void SystemFontCreator::CreateImage(const String& alphabatFileName, const String& textureFileName)
{
	//считать алфавит
	FILE* f = _wfopen(Strings::UTF82Unicode(alphabatFileName).c_str(), L"rb");

	std::wstring str;
	{
		static wchar_t s[1000];
		while(fgetws(s, sizeof(s) / sizeof(s[0]) - 1, f))
		{
			int i;
			for(i = wcslen(s) - 1; i >= 0 && (s[i] == L'\r' || s[i] == L'\n'); --i);
			s[i + 1] = 0;
			str += s;
		}
	}

	fclose(f);

	int n = (int)str.length();

	lf.lfQuality = PROOF_QUALITY;
	HFONT font = CreateFontIndirect(&lf);
	HDC hdc = GetDC(NULL);
	SelectFont(hdc, font);
	std::vector<ptr<File>> images(n);
	std::vector<GLYPHMETRICS> metrics(n);
	//получить изображения символов
	//при этом мы заполняем информацию о символе, кроме текстурных координат
	//полезная информация: как написано в MSDN (статья про GetGlyphOutline)
	//точка отсчета находится на базовой линии шрифта, и положение символа
	//задается относительно нее
	for(int i = 0; i < n; ++i)
		try
		{
			MAT2 mat;
			ZeroMemory(&mat,sizeof(mat));
			mat.eM11.value = 1;
			mat.eM11.fract = 0;
			mat.eM22.value = 1;
			mat.eM22.fract = 0;

			GLYPHMETRICS& gm = metrics[i];
			int size = GetGlyphOutline(hdc, str[i], GGO_GRAY8_BITMAP, &gm, 0, NULL, &mat);
			if(size == GDI_ERROR)
				THROW_PRIMARY_EXCEPTION("Can't get glyph size");
			ptr<File> file = new MemoryFile(size);
			images[i] = file;
			unsigned char* fileData = static_cast<unsigned char*>(file->GetData());
			if(GetGlyphOutline(hdc, str[i], GGO_GRAY8_BITMAP, &gm, size, fileData, &mat) == GDI_ERROR)
				THROW_PRIMARY_EXCEPTION("Can't get glyph");
			
			//как написано в MSDN, в этом режиме возвращаются значения от 0 до 64 включительно
			//так что масштабируем цвета
			for(int j = 0; j < size; ++j)
				fileData[j] = fileData[j] * 255 / 64;
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Can't get symbol image", exception);
		}

	// получить кернинг-пары
	size_t kerningPairsCount = GetKerningPairs(hdc, 0, NULL);
	ptr<File> kerningPairsFile = NEW(MemoryFile(kerningPairsCount * sizeof(KERNINGPAIR)));
	KERNINGPAIR* kp = (KERNINGPAIR*)kerningPairsFile->GetData();
	if(GetKerningPairs(hdc, kerningPairsCount, kp) != kerningPairsCount)
		THROW_PRIMARY_EXCEPTION("Can't get kerning pairs");

	// теперь можно освободить контекст, он больше не нужен
	ReleaseDC(NULL, hdc);

	// кернинг-пары нужно оставить только те, которые работают с символами алфавита
	// составить набор символов алфавита
	std::unordered_set<wchar_t> symbols;
	for(int i = 0; i < n; ++i)
		symbols.insert(str[i]);

	// добавить кернинг-пары
	for(size_t i = 0; i < kerningPairsCount; ++i)
		if(symbols.find(kp[i].wFirst) != symbols.end() && symbols.find(kp[i].wSecond) != symbols.end())
			kerningPairs[std::pair<wchar_t, wchar_t>(kp[i].wFirst, kp[i].wSecond)] = (float)kp[i].iKernAmount;

	// можно освободить память
	kerningPairsFile = nullptr;

	//функтор, который будет вычислять ширину и высоту
	//а также получает размеры одного символа
	//при создании картинки к каждому символу будет добавляться бортик
	//1 пиксел с каждой стороны
	struct Solver
	{
		const std::vector<GLYPHMETRICS>& metrics;
		int n;
		struct Coord
		{
			int x, y;
		};
		std::vector<Coord> coords;
		int width, height;

		Solver(const std::vector<GLYPHMETRICS>& metrics)
			: metrics(metrics), n(metrics.size()), coords(n) {}

		int GetCharWidth(int i) const
		{
			return metrics[i].gmBlackBoxX + 2;
		}

		int GetCharHeight(int i) const
		{
			return metrics[i].gmBlackBoxY + 2;
		}

		void Calculate(int m)
		{
			//вычислить ширину картинки
			width = 0;
			for(int i = 0; i < m; ++i)
				width += GetCharWidth(i);

			//вычислить расположение и высоту символов
			height = 0;
			int lineWidth = 0, lineHeight = 0;
			for(int i = 0; i < n; ++i)
			{
				int charWidth = GetCharWidth(i);
				int charHeight = GetCharHeight(i);
				//если очередной символ не влезает в строку
				if(lineWidth + charWidth > width)
				{
					//перейти к следующей строке
					height += lineHeight;
					lineWidth = charWidth;
					lineHeight = charHeight;
					//указать координаты
					coords[i].x = 0;
					coords[i].y = height;
				}
				//иначе добавить в текущую
				else
				{
					coords[i].x = lineWidth;
					coords[i].y = height;
					lineWidth += charWidth;
					lineHeight = std::max(lineHeight, charHeight);
				}
			}
			//добавить последнюю строку
			height += lineHeight;
		}

		int GetWidth() const
		{
			return width;
		}

		int GetHeight() const
		{
			return height;
		}
	};
	Solver solver(metrics);

	//подобрать ширину и высоту текстуры, чтобы было как можно квадратнее
	//то есть дихотомией, при этом порядок символов менять не пытаемся
	//перебираемый параметр - количество символов в первой строке
	//но для начала выясним ширину самого узкого символа
	int minCharWidth = 0x3fffffff;
	for(int i = 0; i < n; ++i)
		minCharWidth = std::min(minCharWidth, solver.GetCharWidth(i));

	int left = 0, right = n + 1;
	while(left < right - 1)
	{
		int m = (left + right) / 2;
		solver.Calculate(m);
		//получить ширину картинки
		int width = solver.GetWidth();
		//если ширина меньше ширины самого узкого символа, то надо увеличивать
		if(width < minCharWidth)
		{
			left = m;
			continue;
		}
		//получить вычислить высоту картинки
		int height = solver.GetHeight();
		//шаг дихотомии
		if(width < height)
			left = m;
		else
			right = m;
	}

	//все, left содержит количество символов
	//вычислим все еще раз для последнего значения
	solver.Calculate(left);

	int width = min2(solver.GetWidth());
	int height = min2(solver.GetHeight());

	//установим параметры для набора символов
	float pixelWidth = 1.0f / width;
	float pixelHeight = 1.0f / height;
	for(int i = 0; i < n; ++i)
	{
		const GLYPHMETRICS& gm = metrics[i];
		int charWidth = gm.gmBlackBoxX;
		int charHeight = gm.gmBlackBoxY;
		FontChar& fontChar = charset[str[i]];
		fontChar.firstUV = float2(solver.coords[i].x * pixelWidth, (solver.coords[i].y + charHeight + 2) * pixelHeight);
		fontChar.secondUV = float2((solver.coords[i].x + charWidth + 2) * pixelWidth, solver.coords[i].y * pixelHeight);
		//заметим, что тут хитро - Glyph Origin указывает на положение точки отсчета для символа
		//в системе координат с Y, направленной вверх, причем в MSDN этого не написано
		fontChar.screenFirstUV = float2(float(gm.gmptGlyphOrigin.x - 1), float(-charHeight + gm.gmptGlyphOrigin.y - 1));
		fontChar.screenSecondUV = float2(float(gm.gmptGlyphOrigin.x + charWidth + 1), float(gm.gmptGlyphOrigin.y + 1));
		fontChar.width = gm.gmCellIncX;
	}
	//вычислим высоту символов (как максимум из их высот)
	charHeight = 0;
	for(int i = 0; i < n; ++i)
		charHeight = std::max<int>(charHeight, metrics[i].gmBlackBoxY);

	//делаем картинку в формате BMP с палитрой
	int pitch = (width + 3) & ~3;

	//выделим память под картинку
	ptr<File> picture = new MemoryFile(pitch * height);
	char* pictureData = static_cast<char*>(picture->GetData());
	ZeroMemory(pictureData, picture->GetSize());
	//скопируем картинки на буфер
	for(int i = 0; i < n; ++i)
	{
		//получить файл картинки
		ptr<File> image = images[i];
		if(!image->GetSize()) continue;
		char* imageData = static_cast<char*>(image->GetData());
		//скопировать построчно
		//не забываем, что мы добавляем бортик
		int imageWidth = metrics[i].gmBlackBoxX;
		int imageHeight = metrics[i].gmBlackBoxY;
		int imagePitch = (imageWidth + 3) & ~3;
		for(int y = 0; y < imageHeight; ++y)
			memcpy(pictureData + (y + solver.coords[i].y + 1) * pitch + solver.coords[i].x + 1, imageData + y * imagePitch, imageWidth);
	}

	//записать картинку BMP
	{
		ptr<StreamWriter> writer = NEW(StreamWriter(FolderFileSystem::GetNativeFileSystem()->SaveStream(textureFileName)));

		BITMAPFILEHEADER bfh;
		bfh.bfType = 'MB';
		bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) + picture->GetSize();
		bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD);
		bfh.bfReserved1 = 0;
		bfh.bfReserved2 = 0;
		writer->Write(bfh);

		BITMAPINFOHEADER bih;
		ZeroMemory(&bih, sizeof(bih));
		bih.biSize = sizeof(bih);
		bih.biWidth = width;
		bih.biHeight = -height;
		bih.biBitCount = 8;
		bih.biClrUsed = 256;
		bih.biPlanes = 1;
		writer->Write(bih);

		RGBQUAD palette[256];
		for(int i = 0; i < 256; ++i)
		{
			palette[i].rgbRed = i;
			palette[i].rgbGreen = i;
			palette[i].rgbBlue = i;
			palette[i].rgbReserved = 0;
		}
		writer->Write(palette, sizeof(palette));

		writer->Write(pictureData, picture->GetSize());
	}

	this->textureFileName = textureFileName;
}

String SystemFontCreator::GetCommand() const
{
	return "systemfont";
}

void SystemFontCreator::PrintHelp() const
{
	std::cout << "Creates a font file and texture file from system font. Source font will be asked with Select Font dialog. Using:\n";
	std::cout << "systemfont <source alphabat file> <result texture file> <result font file>\n";
}

void SystemFontCreator::Run(const std::vector<String>& arguments)
{
	if(arguments.size() < 3)
		THROW_PRIMARY_EXCEPTION("Must be at least 3 arguments for command");

	GetObject(GetStockFont(DEFAULT_GUI_FONT), sizeof(lf), &lf);
	{
		CHOOSEFONT cf;
		ZeroMemory(&cf, sizeof(cf));
		cf.lStructSize = sizeof(cf);
		cf.lpLogFont = &lf;
		cf.Flags = CF_SCREENFONTS | CF_FORCEFONTEXIST;
		if(!ChooseFont(&cf))
			return;
	}

	CreateImage(arguments[0], arguments[1]);
	MakePointer(NEW(EditableFont(arguments[1], charset, kerningPairs, (float)charHeight)))->Serialize(FolderFileSystem::GetNativeFileSystem()->SaveStream(arguments[2]));
}
