#include "reflection.hpp"

NamedClassMember::NamedClassMember(const char* name) : name(name) {}

const char* NamedClassMember::GetName() const
{
	return name;
}

Class::Class(const char* fullName) : constructor(0), fullName(fullName)
{
	// получить имя класса
	ptrdiff_t i;
	for(i = this->fullName.length() - 1; i >= 0 && fullName[i] != ':'; ++i);
	name = fullName + (++i);
}

Class::~Class()
{
	if(constructor)
		delete constructor;
	for(size_t i = 0; i < methods.size(); ++i)
		delete methods[i];
	for(size_t i = 0; i < staticMethods.size(); ++i)
		delete staticMethods[i];
}
