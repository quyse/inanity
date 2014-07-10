#include "interop.ipp"
#include "../../Exception.hpp"
#include <mono/metadata/appdomain.h>
#include <mono/metadata/assembly.h>
#include <sstream>

BEGIN_INANITY_MONO

//*** class DotNetDomain

DotNetDomain::DotNetDomain(MonoDomain* domain)
: domain(domain) {}

DotNetDomain::~DotNetDomain()
{
//	mono_domain_unload(domain);
}

MonoDomain* DotNetDomain::GetDomain() const
{
	return domain;
}

ptr<DotNetDomain> DotNetDomain::Create()
{
	return NEW(DotNetDomain(mono_domain_create()));
}

ptr<DotNetObject> DotNetDomain::CreateString(const char* str)
{
	return NEW(DotNetObject((MonoObject*)mono_string_new(domain, str)));
}

ptr<DotNetImage> DotNetDomain::LoadAssembly(const char* name)
{
	MonoAssembly* assembly = mono_domain_assembly_open(domain, name);
	if(!assembly)
		THROW(String("Can't load assembly ") + name);
	return NEW(DotNetImage(mono_assembly_get_image(assembly)));
}

//*** class DotNetImage

DotNetImage::DotNetImage(MonoImage* image)
: image(image) {}

DotNetImage::~DotNetImage() {}

ptr<DotNetImage> DotNetImage::GetCorlibImage()
{
	return NEW(DotNetImage(mono_get_corlib()));
}

ptr<DotNetClass> DotNetImage::GetClass(const char* namespaceName, const char* name)
{
	return NEW(DotNetClass(mono_class_from_name(image, namespaceName, name)));
}

//*** class DotNetClass

DotNetClass::DotNetClass(MonoClass* klass)
: klass(klass)
{
	type = mono_class_get_type(klass);
}

DotNetClass::~DotNetClass() {}

MonoClass* DotNetClass::GetClass() const
{
	return klass;
}

ptr<DotNetClass> DotNetClass::GetObjectClass()
{
	return NEW(DotNetClass(mono_get_object_class()));
}

ptr<DotNetObject> DotNetClass::GetTypeObject(ptr<DotNetDomain> domain)
{
	return NEW(DotNetObject((MonoObject*)mono_type_get_object(domain->GetDomain(), type)));
}

ptr<DotNetObject> DotNetClass::CreateObject(ptr<DotNetDomain> domain)
{
	return NEW(DotNetObject(mono_object_new(domain->GetDomain(), klass)));
}

ptr<DotNetArray> DotNetClass::CreateArray(ptr<DotNetDomain> domain, int length)
{
	return NEW(DotNetArray(mono_array_new(domain->GetDomain(), klass, length)));
}

ptr<DotNetField> DotNetClass::GetField(const char* name)
{
	return NEW(DotNetField(this, mono_class_get_field_from_name(klass, name)));
}

ptr<DotNetMethod> DotNetClass::GetMethod(const char* name, int paramsCount)
{
	try
	{
		MonoMethod* method = mono_class_get_method_from_name(klass, name, paramsCount);
		if(!method)
			THROW("Can't get method from name");

		return NEW(DotNetMethod(method));
	}
	catch(Exception* exception)
	{
		std::ostringstream stream;
		stream << "Can't get .NET method " << name << " with " << paramsCount << " argument(s)";
		THROW_SECONDARY(stream.str(), exception);
	}
}

ptr<DotNetConstructor> DotNetClass::GetConstructor(int paramsCount)
{
	return NEW(DotNetConstructor(this, GetMethod(".ctor", paramsCount)));
}

ptr<DotNetProperty> DotNetClass::GetProperty(const char* name)
{
	return NEW(DotNetProperty(mono_class_get_property_from_name(klass, name)));
}

//*** class DotNetObject

DotNetObject::DotNetObject(MonoObject* object)
: object(object)
{
	gcHandle = mono_gchandle_new(object, true);
}

DotNetObject::~DotNetObject()
{
	mono_gchandle_free(gcHandle);
}

MonoObject* DotNetObject::GetObject() const
{
	return object;
}

ptr<DotNetClass> DotNetObject::GetClass()
{
	return NEW(DotNetClass(mono_object_get_class(object)));
}

//*** class DotNetArray

DotNetArray::DotNetArray(MonoArray* array)
: DotNetObject((MonoObject*)array) {}

DotNetArray::~DotNetArray() {}

MonoArray* DotNetArray::GetArray() const
{
	return (MonoArray*)object;
}

//*** class DotNetField

DotNetField::DotNetField(ptr<DotNetClass> klass, MonoClassField* field)
: klass(klass), field(field) {}

DotNetField::~DotNetField() {}

//*** class DotNetMethod

DotNetMethod::DotNetMethod(MonoMethod* method)
: method(method) {}

DotNetMethod::~DotNetMethod() {}

//*** class DotNetConstructor

DotNetConstructor::DotNetConstructor(ptr<DotNetClass> klass, ptr<DotNetMethod> method)
: klass(klass), method(method) {}

DotNetConstructor::~DotNetConstructor() {}

//*** class DotNetProperty

DotNetProperty::DotNetProperty(MonoProperty* property)
: property(property) {}

DotNetProperty::~DotNetProperty() {}

ptr<DotNetMethod> DotNetProperty::GetGetter()
{
	return NEW(DotNetMethod(mono_property_get_get_method(property)));
}

ptr<DotNetMethod> DotNetProperty::GetSetter()
{
	return NEW(DotNetMethod(mono_property_get_set_method(property)));
}

END_INANITY_MONO
