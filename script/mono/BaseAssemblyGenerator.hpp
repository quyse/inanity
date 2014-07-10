#ifndef ___INANITY_SCRIPT_MONO_BASE_ASSEMBLY_GENERATOR_HPP___
#define ___INANITY_SCRIPT_MONO_BASE_ASSEMBLY_GENERATOR_HPP___

#include "mono.hpp"
#include "MetaProvider.hpp"
#include <unordered_map>
#include <mono/metadata/object.h>

BEGIN_INANITY_MONO

class State;
class DotNetDomain;
class DotNetClass;
class DotNetObject;
class DotNetArray;
class DotNetMethod;

class BaseAssemblyGenerator : public Object
{
private:
	ptr<State> state;
	ptr<DotNetDomain> domain;
	ptr<DotNetClass> cString;
	ptr<DotNetMethod> mAddClass;
	ptr<DotNetMethod> mSetClassParent;
	ptr<DotNetMethod> mSetClassConstructor;
	ptr<DotNetMethod> mAddClassMethod;
	ptr<DotNetMethod> mAddClassStaticMethod;
	ptr<DotNetMethod> mResolve;

	ptr<DotNetObject> oAssemblyMeta;

	const char* currentClassFullName;
	ptr<DotNetObject> oCurrentClassFullName;

	ptr<DotNetArray> ConvertArguments(int argumentsCount, const char** argumentTypes);

public:
	BaseAssemblyGenerator(ptr<State> state, const char* libgenPath);
	~BaseAssemblyGenerator();

	/// Register class.
	/** Class becomes current, in order to add members. */
	void Register(MetaProvider::ClassBase* classMeta);

	//*** Registration methods for current class.
	void RegisterConstructor(int argumentsCount, const char** argumentTypes);
	void RegisterFunction(const char* name, const char* returnType, int argumentsCount, const char** argumentTypes);
	void RegisterMethod(const char* name, const char* returnType, int argumentsCount, const char** argumentTypes);

	/// Perform actual generation.
	void Generate(const char* fileName);
};

END_INANITY_MONO

#endif
