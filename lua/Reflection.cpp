#include "Reflection.hpp"

Method::Method(const char* name) : name(name) {}

const char* Method::GetName() const
{
	return name;
}

Class::Class() : constructor(0) {}

Class::~Class()
{
	for(size_t i = 0; i < methods.size(); ++i)
		delete methods[i];
}
