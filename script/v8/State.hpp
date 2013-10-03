#ifndef ___INANITY_SCRIPT_V8_STATE_HPP___
#define ___INANITY_SCRIPT_V8_STATE_HPP___

#include "v8.hpp"
#include "../State.hpp"
#include "v8lib.hpp"
#include <unordered_map>

BEGIN_INANITY_V8

/// The state for V8 virtual machine.
class State : public Inanity::Script::State
{
private:
	v8::Isolate* isolate;
	v8::Persistent<v8::Context> context;

	typedef std::unordered_map<Meta::ClassBase*, v8::Persistent<v8::FunctionTemplate>*> Classes;
	Classes classes;

	/// Set of objects which sent to script.
	typedef std::unordered_map<RefCounted*, v8::Persistent<v8::Object>*> Instances;
	Instances instances;

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
	v8::Local<v8::FunctionTemplate> GetClassTemplate(Meta::ClassBase* classMeta);
	/// Callback for dereferencing object.
	static void InstanceWeakCallback(const v8::WeakCallbackData<v8::Object, Object>& data);
	/// Internal method to unregister instance.
	void InternalUnregisterInstance(Object* object);
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

	/// Unregister instance of object if exposed to script.
	/** Invalidates object instances in script (if they exist), and releases a reference. */
	void UnregisterInstance(RefCounted* object);

	// Script::State's methods.
	void Register(Meta::ClassBase* classMeta);
	ptr<Script::Function> LoadScript(ptr<File> file);

	//******* DON'T CALL EXPLICITLY

	/// Convert object to a v8 value.
	/** Object should be not-null. */
	v8::Local<v8::Object> ConvertObject(Meta::ClassBase* classMeta, RefCounted* object);
	/// Register instance to receive notification when all references from script are gone.
	void InternalRegisterInstance(RefCounted* object, v8::Local<v8::Object> instance);
	/// Process errors from javascript, and throw exceptions.
	void ProcessErrors(const v8::TryCatch& tryCatch);
};

END_INANITY_V8

#endif
