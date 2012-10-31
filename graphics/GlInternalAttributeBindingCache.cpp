#include "GlInternalAttributeBindingCache.hpp"
#include "GlInternalAttributeBinding.hpp"
#include "GlInternalProgram.hpp"
#include "GlContext.hpp"
#include "Layout.hpp"

GlInternalAttributeBindingCache::GlInternalAttributeBindingCache(GlContext* context)
: context(context) {}

ptr<GlInternalAttributeBinding> GlInternalAttributeBindingCache::GetBinding(Layout* vertexLayout, GlInternalProgram* program)
{
	Key key(vertexLayout, program);
	Bindings::const_iterator i = bindings.find(key);
	if(i != bindings.end())
		return i->second;

	ptr<GlInternalAttributeBinding> binding = context->CreateInternalAttributeBinding(vertexLayout, program);

	bindings[key] = binding;

	return binding;
}
