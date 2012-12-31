#ifndef ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___

#include "Value.hpp"
#include "OperationNode.hpp"

BEGIN_INANITY_SHADERS

#define OP2(op) NEW(OperationNode(OperationNode::operation##op, a.GetNode(), b.GetNode()))

// construction values
Value<float4> newfloat4(Value<float3> a, Value<float> b)
{
	return OP2(Float31to4);
}

// dot
template <int n>
Value<vector<n> > dot(Value<vector<n> > a, Value<vector<n> > b)
{
	return OP2(Dot);
}

// cross
Value<float3> cross(Value<float3> a, Value<float3> b)
{
	return OP2(Cross);
}

// mul
template <int n, int m, int k>
Value<matrix<n, k> > mul(Value<matrix<n, m> > a, Value<matrix<m, k> > b)
{
	return OP2(Mul);
}
template <int n, int m>
Value<vector<m> > mul(Value<vector<n> > a, Value<matrix<n, m> > b)
{
	return OP2(Mul);
}
template <int n, int m>
Value<vector<n> > mul(Value<matrix<n, m> > a, Value<vector<m> > b)
{
	return OP2(Mul);
}

#undef OP2

END_INANITY_SHADERS

#endif
