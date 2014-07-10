#include "BaseAssemblyGenerator.hpp"
#include "State.hpp"
#include "interop.ipp"
#include "../../Exception.hpp"

BEGIN_INANITY_MONO

BaseAssemblyGenerator::BaseAssemblyGenerator(ptr<State> state)
: state(state)
{
	BEGIN_TRY();

	domain = NEW(DotNetDomain(state->GetDomain()));
	//domain = DotNetDomain::Create();

	// create interop domain, get some useful things
	ptr<DotNetImage> corlibImage = DotNetImage::GetCorlibImage();

	// get System.Type class
	cType = corlibImage->GetClass("System", "Type");
	// get void class
	cVoid = corlibImage->GetClass("System", "Void");

	// get current domain
	ptr<DotNetClass> cDomain = corlibImage->GetClass("System", "AppDomain");
	ptr<DotNetObject> oDomain = cDomain->GetProperty("CurrentDomain")->GetGetter()->Call(nullptr);

	ptr<DotNetConstructor> ctorAssemblyName = corlibImage->GetClass("System.Reflection", "AssemblyName")
			->GetConstructor(1);
	ptr<DotNetObject> oAssemblyName = ctorAssemblyName->Construct(domain, domain->CreateString("inanity"));

	// create assembly builder
	ptr<DotNetObject> oAssemblyBuilder = cDomain->GetMethod("DefineDynamicAssembly", 2)->Call(
		oDomain,
		oAssemblyName,
		corlibImage->GetClass("System.Reflection.Emit", "AssemblyBuilderAccess")
			->GetField("Save")->GetStatic<int>(domain)
		);
	ptr<DotNetClass> cAssemblyBuilder = oAssemblyBuilder->GetClass();

	// create module builder
	oModuleBuilder = cAssemblyBuilder->GetMethod("DefineDynamicModule", 2)->Call(
		oAssemblyBuilder,
		domain->CreateString("inanity"),
		domain->CreateString("inanity.dll")
	);
	ptr<DotNetClass> cModuleBuilder = oModuleBuilder->GetClass();
	mDefineType = cModuleBuilder->GetMethod("DefineType", 4);

	// get type builder class
	cTypeBuilder = corlibImage->GetClass("System.Reflection.Emit", "TypeBuilder");

	// get method builder class
	cMethodBuilder = corlibImage->GetClass("System.Reflection.Emit", "MethodBuilder");
	mSetCustomAttribute = cMethodBuilder->GetMethod("SetCustomAttribute", 1);

	// get type attributes class and literals
	ptr<DotNetClass> cTypeAttributes = corlibImage->GetClass("System.Reflection", "TypeAttributes");
	int vTypeAttributePublic = cTypeAttributes->GetField("Public")->GetStatic<int>(domain);
	//int vTypeAttributeAbstract = cTypeAttributes->GetField("Abstract")->GetStatic<int>(domain);

	// get method attributes class and literals
	ptr<DotNetClass> cMethodAttributes = corlibImage->GetClass("System.Reflection", "MethodAttributes");
	int vMethodAttributePublic = cMethodAttributes->GetField("Public")->GetStatic<int>(domain);
	int vMethodAttributeHideBySig = cMethodAttributes->GetField("HideBySig")->GetStatic<int>(domain);
	int vMethodAttributeFinal = cMethodAttributes->GetField("Final")->GetStatic<int>(domain);

	// create CustomAttributeBuilder for "internal call" method attribute
	{
		// get InternalCall enum literal
		ptr<DotNetClass> cMethodImplOptions = corlibImage->GetClass("System.Runtime.CompilerServices", "MethodImplOptions");

		int internalCallLiteral = cMethodImplOptions->GetField("InternalCall")->GetStatic<int>(domain);

		// get attribute constructor info
		ptr<DotNetClass> cMethodImplAttribute = corlibImage->GetClass("System.Runtime.CompilerServices", "MethodImplAttribute");
		ptr<DotNetObject> oAttributeConstructorInfo = cType->GetMethod("GetConstructor", 1)->Call(
			cMethodImplAttribute->GetTypeObject(domain),
			cType->CreateArray<ptr<DotNetObject> >(domain,
				cMethodImplOptions->GetTypeObject(domain)
			)
		);

		// get impl options
		ptr<DotNetClass> cObject = DotNetClass::GetObjectClass();
		ptr<DotNetArray> aMethodImplOptions = cObject->CreateArray<ptr<DotNetObject> >(domain,
			cMethodImplOptions->Box(domain, internalCallLiteral)
		);

		// create attribute builder
		oCustomAttributeBuilder = corlibImage->GetClass("System.Reflection.Emit", "CustomAttributeBuilder")
			->GetConstructor(2)->Construct(domain,
				oAttributeConstructorInfo,
				aMethodImplOptions
			);
	}

	// create Inanity.Object class
	{
		// get IDisposable class
		ptr<DotNetClass> cIDisposable = corlibImage->GetClass("System", "IDisposable");

		// define Inanity.Object type
		ptr<DotNetObject> oTypeBuilder = mDefineType->Call(
			oModuleBuilder,
			domain->CreateString("Inanity.Object"),
			vTypeAttributePublic,
			nullptr,
			cType->CreateArray<ptr<DotNetObject> >(domain,
				cIDisposable->GetTypeObject(domain)
			)
		);

		// define Dispose method
		ptr<DotNetObject> oDisposeMethod = cTypeBuilder->GetMethod("DefineMethod", 4)->Call(
			oTypeBuilder,
			domain->CreateString("Dispose"),
			vMethodAttributePublic | vMethodAttributeHideBySig | vMethodAttributeFinal,
			cVoid->GetTypeObject(domain),
			cType->CreateArray<ptr<DotNetObject> >(domain)
		);
		mSetCustomAttribute->Call(oDisposeMethod, oCustomAttributeBuilder);

		cTypeBuilder->GetMethod("CreateType", 0)->Call(oTypeBuilder);

		// save assembly
		cAssemblyBuilder->GetMethod("Save", 1)->Call(
			oAssemblyBuilder,
			domain->CreateString("inanity.dll")
		);
	}

	END_TRY("Can't create base assembly generator");
}

BaseAssemblyGenerator::~BaseAssemblyGenerator() {}

void BaseAssemblyGenerator::Generate(const String& fileName)
{
}

END_INANITY_MONO
