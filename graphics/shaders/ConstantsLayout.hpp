#ifndef ___INANITY_SHADERS_CONSTANTS_LAYOUT_HPP___
#define ___INANITY_SHADERS_CONSTANTS_LAYOUT_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// Класс разметки константных данных.
/** Задаёт, как и где лежат данные, которые предполагается
передавать в шейдеры через константные регистры. */
class ConstantsLayout : public Object
{
public:
	/// Структура одного элемента данных.
	struct Entry
	{
		/// Смещение от начала данных.
		unsigned offset;
		/// Тип данных.
		Type type;
	};
};

END_INANITY_SHADERS

#endif
