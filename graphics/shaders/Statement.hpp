#ifndef ___INANITY_GRAPHICS_SHADERS_STATEMENT_HPP___
#define ___INANITY_GRAPHICS_SHADERS_STATEMENT_HPP___

#include "Expression.hpp"

BEGIN_INANITY_SHADERS

class StatementObject;

/// Класс statement.
/** По сути, это разновидность умного указателя на внутренний объект StatementObject.
Это сделано для того, чтобы его легко можно было бы присваивать или копировать,
и при этом использовать дополнительные операторы. */
class Statement
{
private:
	ptr<StatementObject> object;

public:
	/// Основной конструктор - из указателя на StatementObject.
	Statement(ptr<StatementObject> object = 0);

	/// Конструктор для инкапсуляции выражения в statement.
	Statement(Expression expression);

	/// Получить объект.
	ptr<StatementObject> GetObject() const;

	/// Конкатенация statement'ов, для объединения их в один.
	friend Statement operator,(Statement a, Statement b);
};

END_INANITY_SHADERS

#endif
