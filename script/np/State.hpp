#ifndef ___INANITY_SCRIPT_NP_STATE_HPP___
#define ___INANITY_SCRIPT_NP_STATE_HPP___

#include "MetaProvider.hpp"
#include "../State.hpp"
#include "../../meta/ClassBase.hpp"
#include "../../TypedPool.hpp"
#include "../../ObjectPool.hpp"
#include "../../platform/platform.hpp"
#include "../../platform/npapi.hpp"
#include <unordered_set>
#include <unordered_map>

BEGIN_INANITY_PLATFORM

class NpapiPluginInstance;

END_INANITY_PLATFORM

BEGIN_INANITY_NP

class Any;
class Namespace;
struct NPObjectWrapper;
struct NPObjectNamespace;

class State : public Script::State
{
private:
	Platform::NpapiPluginInstance* pluginInstance;

	/// One static instance of state.
	static State* current;

	typedef std::unordered_set<NPClass*> Classes;
	Classes classes;

	typedef std::unordered_multimap<RefCounted*, NPObjectWrapper*> Instances;
	Instances instances;

	/// Pool of object wrappers.
	ptr<TypedPool<NPObjectWrapper> > npObjectWrapperPool;
	/// Pool of namespace wrappers.
	ptr<TypedPool<NPObjectNamespace> > npObjectNamespacePool;

	/// Pool of script values.
	ptr<ObjectPool<Any> > anyPool;

	/// Root namespace.
	ptr<Namespace> rootNamespace;

public:
	State(Platform::NpapiPluginInstance* pluginInstance);
	~State();

	static State* GetCurrent();

	Platform::NpapiPluginInstance* GetPluginInstance() const;

	//*** Script::State's methods.
	ptr<Function> LoadScript(ptr<File> file);
	void ReclaimInstance(RefCounted* object);
	ptr<Script::Any> NewBoolean(bool boolean);
	ptr<Script::Any> NewNumber(int number);
	ptr<Script::Any> NewNumber(float number);
	ptr<Script::Any> NewNumber(double number);
	ptr<Script::Any> NewString(const String& string);
	ptr<Script::Any> NewArray(int length = 0);
	ptr<Script::Any> NewDict();

	/// Register class.
	void Register(MetaProvider::ClassBase* classMeta);
	template <typename ClassType>
	void Register()
	{
		Register(Meta::MetaOf<MetaProvider, ClassType>());
	}

	/// Check that this is Inanity's class.
	bool CheckClass(NPClass* npClass) const;

	NPObjectWrapper* CreateNPObjectWrapper();
	void InvalidateNPObjectWrapper(NPObjectWrapper* wrapper);
	void DeleteNPObjectWrapper(NPObjectWrapper* wrapper);

	NPObjectNamespace* CreateNPObjectNamespace();
	void DeleteNPObjectNamespace(NPObjectNamespace* wrapper);

	ptr<Any> GetRootNamespace();

	ptr<Any> CreateAny(NPVariant variant);
	NPVariant ConvertObject(MetaProvider::ClassBase* classMeta, RefCounted* object);
	template <typename T>
	ptr<Any> WrapObject(ptr<T> object)
	{
		return CreateAny(ConvertObject(Meta::MetaOf<MetaProvider, T>(), object));
	}
};

END_INANITY_NP

#endif
