#include "State.hpp"
#include "Function.hpp"
#include "thunks.ipp"
#include "MetaProvider.ipp"
#include "../../File.hpp"
#include <sstream>

BEGIN_INANITY_V8

State::State()
{
	// create isolate
	isolate = v8::Isolate::New();
	isolate->SetData(this);

	v8::Isolate::Scope isolateScope(isolate);

	v8::HandleScope handleScope(isolate);

	// create context
	context.Reset(isolate, v8::Context::New(isolate));

#ifdef _DEBUG
	v8::V8::SetCaptureStackTraceForUncaughtExceptions(true);
#endif
}

State::~State()
{
	{
		Scope scope(this);

		for(Classes::iterator i = classes.begin(); i != classes.end(); ++i)
		{
			i->second->Reset();
			delete i->second;
		}
		classes.clear();

		for(Instances::iterator i = instances.begin(); i != instances.end(); ++i)
			InternalReclaimInstance(i);
		instances.clear();
	}

	context.Dispose();
	isolate->Dispose();
}

State::Scope::Scope(State* state) : state(state), handleScope(state->isolate)
{
	state->isolate->Enter();
	v8::Local<v8::Context> context = v8::Local<v8::Context>::New(state->isolate, state->context);
	context->Enter();
}

State::Scope::~Scope()
{
	v8::Local<v8::Context> context = v8::Local<v8::Context>::New(state->isolate, state->context);
	context->Exit();
	state->isolate->Exit();
}

v8::Local<v8::FunctionTemplate> State::GetClassTemplate(MetaProvider::ClassBase* classMeta)
{
	// if class template is already created, return it
	{
		Classes::iterator i = classes.find(classMeta);
		if(i != classes.end())
			return v8::Local<v8::FunctionTemplate>::New(isolate, *i->second);
	}

	// register class

	// create template for global class object
	v8::Local<v8::FunctionTemplate> classTemplate = v8::FunctionTemplate::New();

	// set a name of the class
	classTemplate->SetClassName(v8::String::New(classMeta->GetFullName()));

	// inherit from parent class
	MetaProvider::ClassBase* parentClassMeta = classMeta->GetParent();
	if(parentClassMeta)
		classTemplate->Inherit(GetClassTemplate(parentClassMeta));

	// set constructor callback
	{
		MetaProvider::ConstructorBase* constructor = classMeta->GetConstructor();
		if(constructor)
			classTemplate->SetCallHandler(constructor->GetThunk());
		else
		{
			// set dummy constructor
			// it's actually necessary to use this.
			// FunctionTemplate with zero call handler creates
			// empty objects because internal field wouldn't set.
			classTemplate->SetCallHandler(DummyConstructorThunk);
		}
	}

	// add static methods to template
	const MetaProvider::ClassBase::StaticMethods& staticMethods = classMeta->GetStaticMethods();
	for(size_t i = 0; i < staticMethods.size(); ++i)
	{
		MetaProvider::FunctionBase* function = staticMethods[i];

		classTemplate->Set(
			v8::String::New(function->GetName()),
			v8::FunctionTemplate::New(function->GetThunk())
		);
	}

	// add non-static methods to prototype
	v8::Local<v8::ObjectTemplate> prototypeTemplate = classTemplate->PrototypeTemplate();
	const MetaProvider::ClassBase::Methods& methods = classMeta->GetMethods();
	for(size_t i = 0; i < methods.size(); ++i)
	{
		MetaProvider::MethodBase* method = methods[i];

		prototypeTemplate->Set(
			v8::String::New(method->GetName()),
			v8::FunctionTemplate::New(method->GetThunk())
		);
	}

	// set one memory cell for storing 'this' pointer
	v8::Local<v8::ObjectTemplate> instanceTemplate = classTemplate->InstanceTemplate();
	instanceTemplate->SetInternalFieldCount(1);

	// parse full class name, and store the class object
	{
		const char* fullName = classMeta->GetFullName();

		// current container
		v8::Local<v8::Context> context = v8::Local<v8::Context>::New(isolate, this->context);
		v8::Local<v8::Object> container = context->Global();

		for(size_t i = 0; fullName[i]; )
		{
			size_t j;
			for(j = i + 1; fullName[j] && fullName[j] != '.'; ++j);

			// get a part of the name
			v8::Local<v8::String> pathPart = v8::String::New(fullName + i, j - i);

			// if this is a last part
			if(!fullName[j])
			{
				// put an object into container
				container->Set(pathPart, classTemplate->GetFunction());
				// exit
				break;
			}
			// else it's a transient part
			else
			{
				// get a next container, create if needed
				v8::Local<v8::Value> nextContainer = container->Get(pathPart);
				if(nextContainer->IsUndefined())
				{
					nextContainer = v8::Object::New();
					container->Set(pathPart, nextContainer);
				}

				// move to next container
				container = nextContainer->ToObject();
			}

			i = j + 1;
		}
	}

	// remember class template
	classes.insert(std::make_pair(
		classMeta,
		new v8::Persistent<v8::FunctionTemplate>(isolate, classTemplate)));

	return classTemplate;
}

void State::InternalRegisterInstance(RefCounted* object, v8::Local<v8::Object> instance)
{
#ifdef _DEBUG
	if(instances.find(object) != instances.end())
		THROW("V8 instance already registered");
#endif

	// create persistent handle to get notification when object dies
	v8::Persistent<v8::Object>* persistentObject = new v8::Persistent<v8::Object>(isolate, instance);
	persistentObject->SetWeak(object, &InstanceWeakCallback);

	instances.insert(std::make_pair(object, persistentObject));

	// increase reference count
	object->Reference();
}

void State::InternalUnregisterInstance(RefCounted* object)
{
	// find an object in instances
	Instances::iterator i = instances.find(object);
	if(i != instances.end())
	{
		// object is found

		// reclaim it
		InternalReclaimInstance(i);

		// delete from cache
		instances.erase(i);
	}
}

void State::InternalReclaimInstance(Instances::iterator i)
{
	// clear reference to it from script
	v8::Local<v8::Object> instance = v8::Local<v8::Object>::New(isolate, *i->second);
	instance->SetInternalField(0, v8::External::New(0));

	// destroy persistent handle
	i->second->Reset();
	delete i->second;

	// dereference object
	i->first->Dereference();
}

void State::InstanceWeakCallback(const v8::WeakCallbackData<v8::Object, RefCounted>& data)
{
	State::GetFromIsolate(data.GetIsolate())->InternalUnregisterInstance(data.GetParameter());
}

void State::ProcessErrors(const v8::TryCatch& tryCatch)
{
	if(tryCatch.HasCaught())
	{
		std::ostringstream stream;

		stream << "V8 Javascript Exception\n";

		v8::Local<v8::Message> message = tryCatch.Message();

		// print message
		{
			v8::String::Utf8Value s(message->Get());
			stream << *s << '\n';
		}

		// print source line
		stream << "Source line: ";
		{
			v8::String::Utf8Value s(message->GetSourceLine());
			stream << *s << '\n';
		}

		// print script resource name
		stream << "Script Resource: ";
		{
			v8::String::Utf8Value s(message->GetScriptResourceName());
			stream << *s << '\n';
		}

		// print stack trace
		v8::Local<v8::StackTrace> stackTrace = message->GetStackTrace();
		if(!stackTrace.IsEmpty())
		{
			int frameCount = stackTrace->GetFrameCount();
			if(frameCount)
			{
				stream << "Stack Trace:\n";
				for(int i = 0; i < frameCount; ++i)
				{
					v8::Local<v8::StackFrame> frame = stackTrace->GetFrame(i);

					v8::String::Utf8Value scriptName(frame->GetScriptName());
					v8::String::Utf8Value functionName(frame->GetFunctionName());

					stream << (i + 1) << ')';
					if(scriptName.length())
						stream << ' ' << *scriptName;
					stream << " line " << frame->GetLineNumber();
					stream << " column " << frame->GetColumn();
					if(functionName.length())
						stream << ' ' << *functionName;
					stream << '\n';
				}
			}
		}

		THROW(stream.str());
	}
}

v8::Local<v8::Object> State::ConvertObject(MetaProvider::ClassBase* classMeta, RefCounted* object)
{
	// check if the object is already in cache
	Instances::const_iterator i = instances.find(object);
	if(i != instances.end())
		return v8::Local<v8::Object>::New(isolate, *i->second);

	// get meta
	v8::Local<v8::FunctionTemplate> classTemplate = GetClassTemplate(classMeta);

	// wrap object into external
	v8::Local<v8::Value> external = v8::External::New(object);

	// create an instance of the class
	// constructor thunk is called
	v8::Local<v8::Object> instance = classTemplate->GetFunction()->NewInstance(1, &external);

	return instance;
}

v8::Isolate* State::GetIsolate() const
{
	return isolate;
}

State* State::GetCurrent()
{
	return GetFromIsolate(v8::Isolate::GetCurrent());
}

State* State::GetFromIsolate(v8::Isolate* isolate)
{
	return (State*)isolate->GetData();
}

ptr<Script::Function> State::LoadScript(ptr<File> file)
{
	Scope scope(this);

	v8::TryCatch tryCatch;

	v8::Local<v8::Script> script = v8::Script::Compile(
		v8::String::New(
			(const char*)file->GetData(),
			file->GetSize()));

	ProcessErrors(tryCatch);

	return NEW(Function(this, script));
}

void State::ReclaimInstance(RefCounted* object)
{
	Scope scope(this);

	InternalUnregisterInstance(object);
}

END_INANITY_V8
