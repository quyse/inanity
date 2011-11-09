#ifndef ___RESOURCE_LOADER_HPP___
#define ___RESOURCE_LOADER_HPP___

#include "Object.hpp"

BEGIN_INANITY

/*
Файл содержит абстрактный класс специализированного загрузчика ресурсов.
Подклассы должны реализовывать загрузку файлов своего вида.
*/

class File;
class ResourceLoader : public Object
{
public:
	/// Получить код загружаемых ресурсов.
	virtual int GetResourceCode() const = 0;
	/// Загрузить ресурс.
	virtual ptr<Object> Load(ptr<File> file) = 0;
};

END_INANITY

#endif
