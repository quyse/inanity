#ifndef ___INANITY_GRAPHICS_GEOMETRY_FORMAT_HPP___
#define ___INANITY_GRAPHICS_GEOMETRY_FORMAT_HPP___

#include "GeometrySemantic.hpp"
#include "../String.hpp"
#include <vector>

BEGIN_INANITY

class File;
class OutputStream;
class ResourceLoader;

END_INANITY

BEGIN_INANITY_GRAPHICS

/// Формат геометрии.
/** Задаёт привязки данных.
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
	GeometryFormat(const std::vector<std::pair<String, GeometrySemantic> >& semantics);

	/// Получить семантику.
	const GeometrySemantic& GetSemantic(const String& name) const;

	static ptr<GeometryFormat> Deserialize(ptr<File> file, ResourceLoader* resourceLoader);
	void Serialize(ptr<OutputStream> outputStream);
};

END_INANITY_GRAPHICS

#endif
