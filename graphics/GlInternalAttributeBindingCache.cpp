#include "GlInternalAttributeBindingCache.hpp"
#include "GlInternalAttributeBinding.hpp"
#include "GlDevice.hpp"
#include "Layout.hpp"

BEGIN_INANITY_GRAPHICS

GlInternalAttributeBindingCache::GlInternalAttributeBindingCache(GlDevice* device)
: device(device) {}

ptr<GlInternalAttributeBinding> GlInternalAttributeBindingCache::GetBinding(Layout* vertexLayout)
{
	Bindings::const_iterator i = bindings.find(vertexLayout);
	if(i != bindings.end())
		return i->second;

	ptr<GlInternalAttributeBinding> binding = device->CreateInternalAttributeBinding(vertexLayout);

	bindings[vertexLayout] = binding;

	return binding;
}

END_INANITY_GRAPHICS
