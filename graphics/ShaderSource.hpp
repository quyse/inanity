#ifndef ___INANITY_DX_SHADER_SOURCE_HPP___
#define ___INANITY_DX_SHADER_SOURCE_HPP___

#include "graphics.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY

class File;
class FileSystem;
class ResourceLoader;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Класс исходного кода шейдера.
/** Это текстовый код шейдера на языке HLSL, который, в зависимости от
параметров, может компилироваться в множество различных шейдеров.
Параметры являются макросами, подставляемыми в шейдер при компиляции.
Параметры могут хранить подставляемые строки, а могут и не хранить
(параметры-флажки).
Класс является ресурсом, загружаемым загрузчиком ShaderSourceLoader.
Кэш мультишейдеров использует мультишейдер для того, чтобы компилировать шейдеры.
*/
class ShaderSource : public Object
{
private:
	/// Код шейдера на языке HLSL.
	ptr<File> code;
	/// Файловая система для включаемых файлов (необязательный параметр)
	ptr<FileSystem> includes;
	/// Имя основной функции.
	String functionName;
	/// Профиль компиляции.
	String profile;
	/// Список возможных параметров.
	std::vector<String> parameters;

public:
	ShaderSource(ptr<File> code, ptr<FileSystem> includes, const String& functionName,
		const String& profile, const std::vector<String>& parameters = std::vector<String>());

	ptr<File> GetCode() const;
	ptr<FileSystem> GetIncludes() const;
	const String& GetFunctionName() const;
	const String& GetProfile() const;
	const std::vector<String>& GetParameters() const;
};

END_INANITY_GRAPHICS

#endif
