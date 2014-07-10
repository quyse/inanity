#ifndef ___INANITY_SCRIPT_MONO_STATE_HPP___
#define ___INANITY_SCRIPT_MONO_STATE_HPP___

#include "MetaProvider.hpp"
#include "../State.hpp"
#include "../../meta/ClassBase.hpp"
#include "../../ObjectPool.hpp"
#include <vector>
#include <unordered_map>
#include <mono/metadata/object.h>
#include <mono/metadata/class.h>

BEGIN_INANITY

class Exception;

END_INANITY

BEGIN_INANITY_MONO

class Any;
class Assembly;
class BaseAssemblyGenerator;

/// State of Mono scripting engine.
class State : public Script::State
{
public:
	static State* instance;

private:
	MonoDomain* domain;
	/// Image of base assembly.
	MonoImage* baseImage;
	/// Base class for all Inanity classes.
	MonoClass* baseClass;
	MonoClassField* baseClassPointerField;
	/// Reference queue to receive GC events.
	MonoReferenceQueue* referenceQueue;

	/// List of all classes.
	typedef std::vector<MetaProvider::ClassBase*> Classes;
	Classes classes;

	/// List of all instances.
	typedef std::unordered_multimap<RefCounted*, uint32_t> Instances;
	Instances instances;
	typedef std::unordered_map<uint32_t, ptr<RefCounted> > InstancesByHandle;
	InstancesByHandle instancesByHandle;

	/// Pool of script values.
	ptr<ObjectPool<Any> > anyPool;

	static void ReferenceQueueCallback(void* data);
	void ObjectCollected(uint32_t gcHandle);

	void ReclaimInstanceObject(uint32_t gcHandle);

public:
	/// Create state with specified base assembly.
	/** State will be fully usable only with base assembly.
	If base assembly filename is empty string, state will
	be created in generation-only mode. */
	State(const String& baseAssemblyFileName);
	~State();

	MonoDomain* GetDomain() const;
	MonoImage* GetBaseImage() const;

	ptr<Assembly> LoadAssembly(const String& fileName);

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

	void Register(MetaProvider::ClassBase* classMeta);
	template <typename ClassType>
	void Register()
	{
		Register(Meta::MetaOf<MetaProvider, ClassType>());
	}

	ptr<Any> CreateAny(MonoObject* object);

	/// Check if the object is Inanity object.
	bool IsObjectWrapper(MonoObject* value);
	/// Get pointer to Inanity object from Mono wrapper.
	RefCounted* UnwrapObject(MonoObject* value);
	/// Set pointer for Mono wrapper.
	void SetObjectIntoWrapper(MonoObject* monoObject, RefCounted* object);
	/// Convert Inanity object to Mono wrapper.
	MonoObject* ConvertObject(MetaProvider::ClassBase* classMeta, RefCounted* object);
	/// Raise exception into Mono.
	/** Not returns! Caution needed, because no unwinding is performed. */
	void RaiseException(ptr<Exception> exception);

	void RegisterClassesForGeneration(BaseAssemblyGenerator* generator);
};

END_INANITY_MONO

#endif
