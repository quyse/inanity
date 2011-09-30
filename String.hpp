#ifndef ___INANITY_STRING_HPP___
#define ___INANITY_STRING_HPP___

/*
 * Файл содержит определение типа строки, используемого в Inanity.
 * Основной тип строки в Inanity - это однобайтовая (char) UTF-8 строка.
 */

#include "Object.hpp"
#include <string>

BEGIN_INANITY

typedef std::string String;

END_INANITY

#endif
