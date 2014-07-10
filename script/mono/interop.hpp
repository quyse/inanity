#ifndef ___INANITY_SCRIPT_MONO_INTEROP_HPP___
#define ___INANITY_SCRIPT_MONO_INTEROP_HPP___

#include "mono.hpp"
#include <mono/metadata/object.h>

BEGIN_INANITY_MONO

class DotNetDomain;
class DotNetImage;
class DotNetClass;
class DotNetObject;
class DotNetArray;
class DotNetField;
class DotNetMethod;
class DotNetConstructor;
class DotNetProperty;

class DotNetDomain : public Object
{
private:
	MonoDomain* domain;

public:
	DotNetDomain(MonoDomain* domain);
	~DotNetDomain();

	MonoDomain* GetDomain() const;

	static ptr<DotNetDomain> Create();

	ptr<DotNetObject> CreateString(const char* str);
	ptr<DotNetImage> LoadAssembly(const char* name);
};

class DotNetImage : public Object
{
private:
	MonoImage* image;

public:
	DotNetImage(MonoImage* image);
	~DotNetImage();

	static ptr<DotNetImage> GetCorlibImage();

	ptr<DotNetClass> GetClass(const char* namespaceName, const char* name);
};

class DotNetClass : public Object
{
private:
	MonoClass* klass;
	MonoType* type;

public:
	DotNetClass(MonoClass* klass);
	~DotNetClass();

	MonoClass* GetClass() const;

	static ptr<DotNetClass> GetObjectClass();

	ptr<DotNetObject> GetTypeObject(ptr<DotNetDomain> domain);
	ptr<DotNetObject> CreateObject(ptr<DotNetDomain> domain);
	ptr<DotNetArray> CreateArray(ptr<DotNetDomain> domain, int length);
	template <typename Type, typename... Args>
	ptr<DotNetArray> CreateArray(ptr<DotNetDomain> domain, Args... args);
	template <typename Type>
	ptr<DotNetObject> Box(ptr<DotNetDomain> domain, Type data);
	ptr<DotNetField> GetField(const char* name);
	ptr<DotNetMethod> GetMethod(const char* name, int paramsCount);
	ptr<DotNetConstructor> GetConstructor(int paramsCount);
	ptr<DotNetProperty> GetProperty(const char* name);
};

class DotNetObject : public Object
{
protected:
	MonoObject* object;
	uint32_t gcHandle;

public:
	DotNetObject(MonoObject* object);
	~DotNetObject();

	MonoObject* GetObject() const;

	ptr<DotNetClass> GetClass();
	template <typename Type>
	Type Unbox();
};

class DotNetArray : public DotNetObject
{
public:
	DotNetArray(MonoArray* array);
	~DotNetArray();

	MonoArray* GetArray() const;
	template <typename Type>
	Type Get(int index);
	template <typename Type>
	void Set(int index, Type data);
};

class DotNetField : public Object
{
private:
	ptr<DotNetClass> klass;
	MonoClassField* field;

public:
	DotNetField(ptr<DotNetClass> klass, MonoClassField* field);
	~DotNetField();

	template <typename Type>
	Type GetStatic(ptr<DotNetDomain> domain);
	template <typename Type>
	void SetStatic(ptr<DotNetDomain> domain, Type data);
	template <typename Type>
	Type Get(ptr<DotNetObject> thisObject);
	template <typename Type>
	void Set(ptr<DotNetObject> thisObject, Type data);
};

class DotNetMethod : public Object
{
private:
	MonoMethod* method;

	template <typename... Args>
	static MonoObject* CallHelper(MonoMethod* method, MonoObject* thisObject, Args... args, MonoObject** exception);

public:
	DotNetMethod(MonoMethod* method);
	~DotNetMethod();

	template <typename... Args>
	ptr<DotNetObject> Call(ptr<DotNetObject> thisObject, Args... args);
};

class DotNetConstructor : public Object
{
private:
	ptr<DotNetClass> klass;
	ptr<DotNetMethod> method;

public:
	DotNetConstructor(ptr<DotNetClass> klass, ptr<DotNetMethod> method);
	~DotNetConstructor();

	template <typename... Args>
	ptr<DotNetObject> Construct(ptr<DotNetDomain> domain, Args... args);
};

class DotNetProperty : public Object
{
private:
	MonoProperty* property;

public:
	DotNetProperty(MonoProperty* property);
	~DotNetProperty();

	ptr<DotNetMethod> GetGetter();
	ptr<DotNetMethod> GetSetter();
};

END_INANITY_MONO

#endif
