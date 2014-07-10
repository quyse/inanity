#ifndef ___INANITY_SCRIPT_MONO_BASE_ASSEMBLY_GENERATOR_HPP___
#define ___INANITY_SCRIPT_MONO_BASE_ASSEMBLY_GENERATOR_HPP___

#include "mono.hpp"
#include "MetaProvider.hpp"
#include "../../String.hpp"
#include <unordered_map>
#include <mono/metadata/object.h>

BEGIN_INANITY_MONO

class State;
class DotNetDomain;
class DotNetClass;
class DotNetObject;
class DotNetMethod;

class BaseAssemblyGenerator : public Object
{
private:
	ptr<State> state;
	ptr<DotNetDomain> domain;

	/// Method information for generation.
	struct Method
	{
		String name;
		/// Does any derived class override this method.
		bool isOverrided;
	};

	/// Class information for generation.
	struct Class
	{
		String fullName;
		std::unordered_map<MetaProvider::MethodBase*, Method> methods;
	};
	typedef std::unordered_map<MetaProvider::ClassBase*, Class> Classes;
	Classes classes;

	//*** Things for generation.
	ptr<DotNetClass> cType;
	ptr<DotNetClass> cVoid;
	ptr<DotNetClass> cTypeBuilder;
	ptr<DotNetClass> cMethodBuilder;
	ptr<DotNetMethod> mSetCustomAttribute;
	ptr<DotNetObject> oModuleBuilder;
	ptr<DotNetMethod> mDefineType;
	ptr<DotNetObject> oCustomAttributeBuilder;

public:
	BaseAssemblyGenerator(ptr<State> state);
	~BaseAssemblyGenerator();

	void Register(MetaProvider::ClassBase* classMeta);

	void Generate(const String& fileName);
};

END_INANITY_MONO

#endif
