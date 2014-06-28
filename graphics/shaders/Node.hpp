#ifndef ___INANITY_GRAPHICS_SHADERS_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_NODE_HPP___

#include "shaders.hpp"

BEGIN_INANITY_SHADERS

/// Abstract node of expression tree.
class Node : public Object
{
public:
	/// Node types.
	enum Type
	{
		typeFloatConst,
		typeIntConst,
		typeAttribute,
		typeUniform,
		typeSampler,
		/// Node to read uniform non-array variable.
		typeReadUniform,
		/// Node to read uniform array variable.
		typeIndexUniformArray,
		/// Transformed variable.
		/** Output vertex or input pixel variable. */
		typeTransformed,
		/// Output vertex variable.
		typeInterpolate,
		/// Several expressions in order.
		typeSequence,
		/// Vector component swizzle operation.
		typeSwizzle,
		/// General operation.
		typeOperation,
		/// General action,
		typeAction,
		/// Texture sample.
		typeSample,
		/// Fragment output.
		typeFragment,
		/// Casting.
		typeCast
	};

public:
	virtual Type GetType() const = 0;
};

END_INANITY_SHADERS

#endif
