#ifndef ___INANITY_GRAPHICS_SHADERS_SWIZZLE_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_SWIZZLE_NODE_HPP___

#include "ValueNode.hpp"

BEGIN_INANITY_SHADERS

/// Node for swizzling vector components.
class SwizzleNode : public ValueNode
{
private:
	ptr<ValueNode> a;
	/// Swizzle string.
	/** Contains from 1 to 4 symbols x, y, z, w.
	Should be in static memory. */
	const char* map;

public:
	SwizzleNode(ptr<ValueNode> a, const char* map);

	Type GetType() const;
	DataType GetValueType() const;

	ptr<ValueNode> GetA() const;
	const char* GetMap() const;
};

END_INANITY_SHADERS

#endif
