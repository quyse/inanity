#include "BaseAssemblyGenerator.hpp"
#include "State.hpp"
#include "MetaProvider.ipp"
#include "interop.ipp"
#include "../../meta/ClassBase.ipp"
#include "../../Exception.hpp"

BEGIN_INANITY_MONO

BaseAssemblyGenerator::BaseAssemblyGenerator(ptr<State> state, const char* libgenPath)
: state(state), currentClassFullName(nullptr)
{
	BEGIN_TRY();

	domain = NEW(DotNetDomain(state->GetDomain()));

	// load libgen assembly
	ptr<DotNetImage> libgenImage = domain->LoadAssembly(libgenPath);

	// get string class
	cString = DotNetImage::GetCorlibImage()->GetClass("System", "String");

	// get AssemblyMeta class
	ptr<DotNetClass> cAssemblyMeta = libgenImage->GetClass("Inanity.Script.Mono", "AssemblyMeta");

	// get some methods
	mAddClass = cAssemblyMeta->GetMethod("AddClass", 1);
	mSetClassParent = cAssemblyMeta->GetMethod("SetClassParent", 2);
	mSetClassConstructor = cAssemblyMeta->GetMethod("SetClassConstructor", 2);
	mAddClassMethod = cAssemblyMeta->GetMethod("AddClassMethod", 4);
	mAddClassStaticMethod = cAssemblyMeta->GetMethod("AddClassStaticMethod", 4);
	mResolve = cAssemblyMeta->GetMethod("Resolve", 2);

	// create instance of assembly meta
	oAssemblyMeta = cAssemblyMeta->GetConstructor(0)->Construct(domain);

	END_TRY("Can't create base assembly generator");
}

BaseAssemblyGenerator::~BaseAssemblyGenerator() {}

ptr<DotNetArray> BaseAssemblyGenerator::ConvertArguments(int argumentsCount, const char** argumentTypes)
{
	ptr<DotNetArray> aArgumentTypes = cString->CreateArray(domain, argumentsCount);
	for(int i = 0; i < argumentsCount; ++i)
		aArgumentTypes->Set(i, domain->CreateString(argumentTypes[i]));
	return aArgumentTypes;
}

void BaseAssemblyGenerator::Register(MetaProvider::ClassBase* classMeta)
{
	currentClassFullName = classMeta->GetFullName();
	oCurrentClassFullName = domain->CreateString(currentClassFullName);
	mAddClass->Call(oAssemblyMeta, oCurrentClassFullName);

	MetaProvider::ClassBase* parentClassMeta = classMeta->GetParent();
	if(parentClassMeta)
		mSetClassParent->Call(oAssemblyMeta,
			oCurrentClassFullName,
			domain->CreateString(parentClassMeta->GetFullName())
		);

	MetaProvider::ConstructorBase* constructorMeta = classMeta->GetConstructor();
	if(constructorMeta)
		constructorMeta->RegisterForGeneration(this);

	const MetaProvider::ClassBase::Methods& methods = classMeta->GetMethods();
	for(size_t i = 0; i < methods.size(); ++i)
		methods[i]->RegisterForGeneration(this);

	const MetaProvider::ClassBase::StaticMethods& staticMethods = classMeta->GetStaticMethods();
	for(size_t i = 0; i < staticMethods.size(); ++i)
		staticMethods[i]->RegisterForGeneration(this);
}

void BaseAssemblyGenerator::RegisterConstructor(int argumentsCount, const char** argumentTypes)
{
	mSetClassConstructor->Call(oAssemblyMeta,
		oCurrentClassFullName,
		ConvertArguments(argumentsCount, argumentTypes)
	);
}

void BaseAssemblyGenerator::RegisterFunction(const char* name, const char* returnType, int argumentsCount, const char** argumentTypes)
{
	mAddClassStaticMethod->Call(oAssemblyMeta,
		oCurrentClassFullName,
		domain->CreateString(name),
		domain->CreateString(returnType),
		ConvertArguments(argumentsCount, argumentTypes)
	);
}

void BaseAssemblyGenerator::RegisterMethod(const char* name, const char* returnType, int argumentsCount, const char** argumentTypes)
{
	mAddClassMethod->Call(oAssemblyMeta,
		oCurrentClassFullName,
		domain->CreateString(name),
		domain->CreateString(returnType),
		ConvertArguments(argumentsCount, argumentTypes)
	);
}

void BaseAssemblyGenerator::Generate(const char* fileName)
{
	mResolve->Call(oAssemblyMeta,
		domain->CreateString("inanity"),
		domain->CreateString(fileName)
	);
}

END_INANITY_MONO
