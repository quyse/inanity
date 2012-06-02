#include "Language.hpp"

BEGIN_INANITY_SHADERS

#define DEFINE_TYPE_NAME(t) \
	template <> const char* GetTypeName<t>() { return #t; }
DEFINE_TYPE_NAME(float)
DEFINE_TYPE_NAME(float2)
DEFINE_TYPE_NAME(float3)
DEFINE_TYPE_NAME(float4)
DEFINE_TYPE_NAME(float4x4)
#undef DEFINE_TYPE_NAME

END_INANITY_SHADERS

Composer::Composer()
{
#ifdef _DEBUG
	if(current)
		THROW_PRIMARY_EXCEPTION("Using of more then one ShaderLanguage::Composer`s at a time is not allowed");
#endif
	current = this;
}

Composer::~Composer()
{
	current = nullptr;
}

Composer* Composer::getCurrent()
{
	if(!current)
		THROW_PRIMARY_EXCEPTION("No current composer");
	return current;
}

std::ostringstream& Composer::declarationStream()
{
	return declarations;
}

std::ostringstream& Composer::statementStream()
{
	return statements;
}

std::string Composer::finalize() const
{
	std::ostringstream res;
	res << "struct Input {\n";
	for(size_t i = 0; i < inputDeclarations.size(); ++i)
		res << inputDeclarations[i].second;
	res << "};\n";
	res << "struct Output {\n";
	for(size_t i = 0; i < outputDeclarations.size(); ++i)
		res << outputDeclarations[i].second;
	res << "};\n";
	res << "Output F(Input input) {\nOutput output;\n" << declarations.str() << statements.str() << "return output;\n}\n";
	return res.str();
}
