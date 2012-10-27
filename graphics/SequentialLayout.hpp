#ifndef ___INANITY_GRAPHICS_SEQUENTIAL_LAYOUT_HPP___
#define ___INANITY_GRAPHICS_SEQUENTIAL_LAYOUT_HPP___

#include "Layout.hpp"
#include <vector>

BEGIN_INANITY_GRAPHICS

/// Класс разметки последовательности данных.
/** По сравнению с классом Layout содержит дополнительное поле stride. */
class SequentialLayout : public Layout
{
private:
	/// Расстояние в байтах между началами буферов.
	int stride;

public:
	SequentialLayout(const std::vector<Element>& elements, int size, int stride);

	int GetStride() const;
};

END_INANITY_GRAPHICS

#endif
