#ifndef ___INANITY_GRAPHICS_SHADERS_INSTANCER_HPP___
#define ___INANITY_GRAPHICS_SHADERS_INSTANCER_HPP___

#include "Value.hpp"

BEGIN_INANITY_GRAPHICS

class Device;
class Context;
class AttributeLayout;
class VertexBuffer;
class AttributeLayoutSlot;

END_INANITY_GRAPHICS

BEGIN_INANITY_SHADERS

class UniformGroup;
class Node;

/// Instancer helper object.
/** Abstracts from device capabilities in order to provide instancing.
All instance data should be uploaded to uniform arrays as usual.
This object only abstracts instanceID in three ways:
* hardware shader instancing: hardware instanceID
* hardware attribute instancing: instanced vertex attribute instanceID
* fallback: several draw calls with changing instanceID in a separate uniform buffer
*/

class Instancer : public Object
{
private:
	enum Mode
	{
		modeDrawInstancing,
		modeAttributeInstancing,
		modeFallback
	} mode;

	int maximumInstancesCount;

	// For hardware instancing all is easy.

	//** For attribute instancing.

	ptr<VertexBuffer> vb;
	ptr<AttributeLayoutSlot> als;

	//** For fallback mode.

	/// Uniform group for an instanceID.
	ptr<UniformGroup> ugInstanceID;
	/// Number of uniform buffer slot used.
	static const int uniformBufferSlot = 7;


	/// Instance ID shader value.
	ptr<Node> nodeInstanceID;

public:
	Instancer(ptr<Device> device, int maximumInstancesCount, ptr<AttributeLayout> attributeLayout);

	Value<uint> GetInstanceID() const;

	void Draw(Context* context, int instancesCount);
};

END_INANITY_SHADERS

#endif
