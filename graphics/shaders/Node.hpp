#ifndef ___INANITY_GRAPHICS_SHADERS_NODE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_NODE_HPP___

#include "shaders.hpp"
#include "../DataType.hpp"

BEGIN_INANITY_SHADERS

/// Abstract node of expression tree.
class Node : public Object
{
public:
	/// Expression types.
	/** Corresponds to a class node can be casted to. */
	enum Type
	{
		typeFloatConst,
		typeIntConst,
		typeAttribute,
		typeUniform,
		typeSampler,
		/// Temporary variable.
		typeTemp,
		/// Transformed variable.
		/** Output vertex or input pixel variable. */
		typeTransformed,
		/// Several expressions in order.
		typeSequence,
		/// Vector component swizzle operation.
		typeSwizzle,
		/// General operation.
		typeOperation,
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
