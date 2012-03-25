#include "FontCreator.h"

FontCreator::FontCreator(const std::wstring& textureName, float charWidth, float charHeight)
: textureName(textureName), charWidth(charWidth), charHeight(charHeight)
{
}

void FontCreator::Generate256Symbols()
{
	const float charSize = 1.0f / 16;
	for(wchar_t i = 0; i < 256; ++i)
	{
		FontChar c;
		float x = charSize * (i % 16);
		float y = charSize * (i / 16);
		c.firstUV = D3DXVECTOR2(x, y);
		c.secondUV = D3DXVECTOR2(x + charSize, y + charSize);
		c.screenFirstUV = D3DXVECTOR2(0, 0);
		c.screenSecondUV = D3DXVECTOR2(charSize, charSize);
		c.width = charWidth;
		charset[i] = c;
	}
}

void FontCreator::GenerateFromAlphabat(const wchar_t* alphabatFileName)
{
	FILE* f = _wfopen(alphabatFileName, L"rb");

	static wchar_t s[1000];

	std::vector<std::wstring> strs;
	while(fgetws(s, sizeof(s) / sizeof(s[0]) - 1, f))
		strs.push_back(s);

	fclose(f);

	float oneCharHeight = 1.0f / strs.size();
	for(unsigned i = 0; i < strs.size(); ++i)
	{
		strs[i] = strs[i].substr(0, strs[i].length() - 2);
		int length = strs[i].length();
		float oneCharWidth = 1.0f / length;
		for(int j = 0; j < length; ++j)
		{
			FontChar c;
			c.firstUV = D3DXVECTOR2(j * oneCharWidth, (i + 1) * oneCharHeight);
			c.secondUV = D3DXVECTOR2((j + 1) * oneCharWidth, i * oneCharHeight);
			c.screenFirstUV = D3DXVECTOR2(0, 0);
			c.screenSecondUV = D3DXVECTOR2(charWidth, charHeight);
			c.width = charWidth;
			charset[strs[i][j]] = c;
		}
	}
}

ptr<File> FontCreator::Save()
{
	return FontFormatter::Save(new EditableFont(textureName, charset, charHeight));
}
