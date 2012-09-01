#ifndef ___INANITY_GRAPHICS_SHADERS_STATEMENT_OBJECT_HPP___
#define ___INANITY_GRAPHICS_SHADERS_STATEMENT_OBJECT_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// Абстрактный класс объекта statement.
class StatementObject : public Object
{
public:
	/// Тип statement'а.
	enum Type
	{
		typeExpression,
		typeComma,
		typeIf
	};

public:
	virtual Type GetType() const = 0;
};

END_INANITY_SHADERS

#endif
