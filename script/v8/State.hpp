#ifndef ___INANITY_SCRIPT_V8_STATE_HPP___
#define ___INANITY_SCRIPT_V8_STATE_HPP___

#include "v8.hpp"
#include "MetaProvider.hpp"
#include "../State.hpp"
#include "../../ObjectPool.hpp"
#include "v8lib.hpp"
#include <unordered_map>

BEGIN_INANITY_V8

class Any;

/// The state for V8 virtual machine.
class State : public Inanity::Script::State
{
private:
	v8::Isolate* isolate;
	v8::Persistent<v8::Context> context;

	typedef std::unordered_map<MetaProvider::ClassBase*, v8::Persistent<v8::FunctionTemplate>*> Classes;
	Classes classes;

	/// Set of objects which sent to script.
	typedef std::unordered_map<RefCounted*, v8::Persistent<v8::Object>*> Instances;
	Instances instances;

	/// Pool of script values.
	ptr<ObjectPool<Any> > anyPool;

public:
	class Scope
	{
	private:
		State* state;
		v8::HandleScope handleScope;

	public:
		Scope(State* state);
		~Scope();
	};

private:
	/// Get a function template for class.
	/** Register if needed. */
	v8::Local<v8::FunctionTemplate> GetClassTemplate(MetaProvider::ClassBase* classMeta);
	/// Callback for dereferencing object.
	static void InstanceWeakCallback(const v8::WeakCallbackData<v8::Object, RefCounted>& data);
	/// Internal method to unregister instance.
	void InternalUnregisterInstance(RefCounted* object);
	/// Dereference object and clear references to it from script.
	void InternalReclaimInstance(Instances::iterator i);

public:
	State();
	~State();

	v8::Isolate* GetIsolate() const;

	/// Get current entered state.
	static State* GetCurrent();
	/// Get state from v8::Isolate.
	static State* GetFromIsolate(v8::Isolate* isolate);

	/// Register class.
	template <typename ClassType>
	void Register()
	{
		Scope scope(this);
		GetClassTemplate(Meta::MetaOf<MetaProvider, ClassType>());
	}

	// Script::State's methods.
	ptr<Script::Function> LoadScript(ptr<File> file);
	void ReclaimInstance(RefCounted* object);
	ptr<Script::Any> NewNumber(float number);
	ptr<Script::Any> NewNumber(double number);
	ptr<Script::Any> NewArray(int length = 0);

	//******* DON'T CALL EXPLICITLY

	/// Create script value.
	ptr<Any> CreateAny(v8::Local<v8::Value> value);
	/// Convert object to a v8 value.
	/** Object should be not-null. */
	v8::Local<v8::Object> ConvertObject(MetaProvider::ClassBase* classMeta, RefCounted* object);
	/// Register instance to receive notification when all references from script are gone.
	void InternalRegisterInstance(RefCounted* object, v8::Local<v8::Object> instance);
	/// Process errors from javascript, and throw exceptions.
	void ProcessErrors(const v8::TryCatch& tryCatch);
};

END_INANITY_V8

#endif
