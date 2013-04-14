#ifndef ___INANITY_GRAPHICS_GL_INTERNAL_ATTRIBUTE_BINDING_CACHE_HPP___
#define ___INANITY_GRAPHICS_GL_INTERNAL_ATTRIBUTE_BINDING_CACHE_HPP___

#include "graphics.hpp"
#include "opengl.hpp"
#include <unordered_map>

BEGIN_INANITY_GRAPHICS

class GlInternalAttributeBinding;
class Layout;
class GlDevice;

/// Кэш привязок OpenGL.
class GlInternalAttributeBindingCache : public Object
{
private:
	typedef std::unordered_map<Layout*, ptr<GlInternalAttributeBinding> > Bindings;
	Bindings bindings;

	GlDevice* device;

public:
	GlInternalAttributeBindingCache(GlDevice* device);

	ptr<GlInternalAttributeBinding> GetBinding(Layout* vertexLayout);
};

END_INANITY_GRAPHICS

#endif
