#ifndef ___INANITY_GRAPHICS_SHADERS_EXPRESSION_HPP___
#define ___INANITY_GRAPHICS_SHADERS_EXPRESSION_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

class Node;

/// Класс выражения. Хранит указатель на узел выражения.
class Expression
{
protected:
	/// Объект выражения.
	ptr<Node> node;

private:
	/// Оператор присваивания запрещён (приватный).
	void operator=(Expression a);

public:
	Expression(ptr<Node> node);

	ptr<Node> GetNode() const;
};

Expression operator,(Expression a, Expression b);

END_INANITY_SHADERS

#endif
