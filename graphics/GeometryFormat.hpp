#ifndef ___INANITY_GRAPHICS_GEOMETRY_FORMAT_HPP___
#define ___INANITY_GRAPHICS_GEOMETRY_FORMAT_HPP___

#include "GeometrySemantic.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

/// Формат геометрии.
/** Задаёт привязки данных. Является ресурсом, загружаемым загрузчиком
GeometryFormatLoader.
Семантика, то есть поле в структуре вершины.
Семантика имеет имя, которое используется в шейдере в качестве семантики поля структуры.
Сама семантика является просто именем, типа Normal.
*/
class GeometryFormat : public Object
{
private:
	/// Список семантик геометрии по именам.
	std::vector<std::pair<String, GeometrySemantic> > semantics;

public:
	enum
	{
		resourceCode = 'geof'
	};

	GeometryFormat(const std::vector<std::pair<String, GeometrySemantic> >& semantics);

	/// Получить семантику.
	const GeometrySemantic& GetSemantic(const String& name) const;
};

END_INANITY_GRAPHICS

#endif
