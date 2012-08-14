#ifndef ___INANITY_GRAPHICS_GL_INTERNAL_ATTRIBUTE_BINDING_CACHE_HPP___
#define ___INANITY_GRAPHICS_GL_INTERNAL_ATTRIBUTE_BINDING_CACHE_HPP___

#include "graphics.hpp"
#include "opengl.hpp"
#include <unordered_map>

BEGIN_INANITY_GRAPHICS

class GlInternalAttributeBinding;
class VertexLayout;
class GlInternalProgram;
class GlContext;

/// Кэш привязок OpenGL.
class GlInternalAttributeBindingCache : public Object
{
private:
	struct Key
	{
		VertexLayout* vertexLayout;
		GlInternalProgram* program;
		Key(VertexLayout* vertexLayout, GlInternalProgram* program);
		operator size_t() const;
	};
	typedef std::unordered_map<Key, ptr<GlInternalAttributeBinding> > Bindings;
	Bindings bindings;

	GlContext* context;

public:
	GlInternalAttributeBindingCache(GlContext* context);

	ptr<GlInternalAttributeBinding> GetBinding(VertexLayout* vertexLayout, GlInternalProgram* program);
};

END_INANITY_GRAPHICS

#endif
