#include "State.hpp"
#include "Function.hpp"
#include "../../meta/Constructor.ipp"
#include "../../meta/Function.ipp"
#include "../../meta/Method.ipp"
#include "../../File.hpp"

BEGIN_INANITY_V8

State::State()
{
	// create isolate
	isolate = v8::Isolate::New();

	v8::HandleScope handleScope(isolate);

	// create context
	context.Reset(isolate, v8::Context::New(isolate));
}

State::~State()
{
	context.Dispose();
	isolate->Dispose();
}

State::Scope::Scope(State* state) : state(state)
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

v8::Isolate* State::GetIsolate() const
{
	return isolate;
}

void State::Register(Meta::ClassBase* classMeta)
{
	Scope scope(this);

	// create template for global class object
	v8::Handle<v8::FunctionTemplate> classTemplate = v8::FunctionTemplate::New();

	// set a name of the class
	classTemplate->SetClassName(v8::String::New(classMeta->GetFullName()));

	// set constructor callback
	{
		Meta::ConstructorBase* constructor = classMeta->GetConstructor();
		if(constructor)
		{
			ConstructorExtensionBase* extension = constructor->GetV8Extension();
			classTemplate->SetCallHandler(extension->GetThunk());
		}
	}

	// add static methods to template
	const Meta::ClassBase::StaticMethods& staticMethods = classMeta->GetStaticMethods();
	for(size_t i = 0; i < staticMethods.size(); ++i)
	{
		Meta::FunctionBase* function = staticMethods[i];

		FunctionExtensionBase* extension = function->GetV8Extension();

		classTemplate->Set(
			v8::String::New(function->GetName()),
			v8::FunctionTemplate::New(extension->GetThunk())
		);
	}

	// add non-static methods to prototype
	v8::Handle<v8::ObjectTemplate> prototypeTemplate = classTemplate->PrototypeTemplate();
	const Meta::ClassBase::Methods& methods = classMeta->GetMethods();
	for(size_t i = 0; i < methods.size(); ++i)
	{
		Meta::MethodBase* method = methods[i];

		MethodExtensionBase* extension = method->GetV8Extension();

		prototypeTemplate->Set(
			v8::String::New(method->GetName()),
			v8::FunctionTemplate::New(extension->GetThunk())
		);
	}

	// parse full class name, and store the class object
	{
		const char* fullName = classMeta->GetFullName();

		// current container
		v8::Handle<v8::Context> context = v8::Handle<v8::Context>::New(isolate, this->context);
		v8::Handle<v8::Object> container = context->Global();

		for(size_t i = 0; fullName[i]; )
		{
			size_t j;
			for(j = i + 1; fullName[j] && fullName[j] != '.'; ++j);

			// get a part of the name
			v8::Handle<v8::String> pathPart = v8::String::New(fullName + i, j - i);

			// if this is a last part
			if(!fullName[j])
			{
				// put an object into container
				container->Set(pathPart, classTemplate->GetFunction());
			}
			// else it's a transient part
			else
			{
				// get a next container, create if needed
				v8::Handle<v8::Value> nextContainer = container->Get(pathPart);
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
}

ptr<Script::Function> State::LoadScript(ptr<File> file)
{
	Scope scope(this);

	v8::Handle<v8::Script> script = v8::Script::Compile(
		v8::String::New(
			(const char*)file->GetData(),
			file->GetSize()));

	return NEW(Function(this, script));
}

END_INANITY_V8
