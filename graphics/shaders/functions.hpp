#ifndef ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___

#include "Value.hpp"
#include "OperationNode.hpp"

BEGIN_INANITY_SHADERS

#define OP1(op) NEW(OperationNode(OperationNode::operation##op, a.GetNode()))
#define OP2(op) NEW(OperationNode(OperationNode::operation##op, a.GetNode(), b.GetNode()))
#define OP4(op) NEW(OperationNode(OperationNode::operation##op, a.GetNode(), b.GetNode(), c.GetNode(), d.GetNode()))

// construction values
Value<float4> newfloat4(Value<float> a, Value<float> b, Value<float> c, Value<float> d)
{
	return OP4(Float1111to4);
}
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

// normalize
template <int n>
Value<vector<n> > normalize(Value<vector<n> > a)
{
	return OP1(Normalize);
}

#undef OP2

END_INANITY_SHADERS

#endif
