#ifndef ___INANITY_SCRIPT_NP_STATE_HPP___
#define ___INANITY_SCRIPT_NP_STATE_HPP___

#include "MetaProvider.hpp"
#include "../State.hpp"
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
struct NPObjectWrapper;

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

	/// Pool of script values.
	ptr<ObjectPool<Any> > anyPool;

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
	ptr<Script::Any> WrapObject(ptr<RefCounted> object);

	bool CheckClass(NPClass* npClass) const;
	NPObjectWrapper* CreateNPObjectWrapper();
	void InvalidateNPObjectWrapper(NPObjectWrapper* wrapper);
	void DeleteNPObjectWrapper(NPObjectWrapper* wrapper);
	ptr<Any> CreateAny(NPVariant variant);
	NPVariant ConvertObject(MetaProvider::ClassBase* classMeta, RefCounted* object);
};

END_INANITY_NP

#endif
