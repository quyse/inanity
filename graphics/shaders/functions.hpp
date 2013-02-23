#ifndef ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___

#include "Value.hpp"
#include "OperationNode.hpp"

BEGIN_INANITY_SHADERS

#define OP0(op) ptr<Node>(NEW(OperationNode(OperationNode::operation ## op)))
#define OP1(op) ptr<Node>(NEW(OperationNode(OperationNode::operation ## op, a.GetNode())))
#define OP2(op) ptr<Node>(NEW(OperationNode(OperationNode::operation ## op, a.GetNode(), b.GetNode())))
#define OP3(op) ptr<Node>(NEW(OperationNode(OperationNode::operation ## op, a.GetNode(), b.GetNode(), c.GetNode())))
#define OP4(op) ptr<Node>(NEW(OperationNode(OperationNode::operation ## op, a.GetNode(), b.GetNode(), c.GetNode(), d.GetNode())))

// construction values
inline Value<float2> newfloat2(Value<float> a, Value<float> b) { return OP2(Float11to2); }
inline Value<float3> newfloat3(Value<float> a, Value<float> b, Value<float> c) { return OP3(Float111to3); }
inline Value<float4> newfloat4(Value<float> a, Value<float> b, Value<float> c, Value<float> d) { return OP4(Float1111to4); }
inline Value<float4> newfloat4(Value<float3> a, Value<float> b) { return OP2(Float31to4); }
inline Value<float4> newfloat4(Value<float2> a, Value<float> b, Value<float> c) { return OP3(Float211to4); }

// setPosition
inline Expression setPosition(Value<float4> a) { return OP1(SetPosition); }
// getInstanceID
inline Value<uint> getInstanceID() { return OP0(GetInstanceID); }

// dot
template <int n>
inline Value<float> dot(Value<vector<n> > a, Value<vector<n> > b) { return OP2(Dot); }

// cross
inline Value<float3> cross(Value<float3> a, Value<float3> b) { return OP2(Cross); }

// mul
template <int n, int m, int k>
inline Value<matrix<n, k> > mul(Value<matrix<n, m> > a, Value<matrix<m, k> > b) { return OP2(Mul); }
template <int n, int m>
inline Value<vector<m> > mul(Value<vector<n> > a, Value<matrix<n, m> > b) { return OP2(Mul); }
template <int n, int m>
inline Value<vector<n> > mul(Value<matrix<n, m> > a, Value<vector<m> > b) { return OP2(Mul); }

// length
template <int n>
inline Value<float> length(Value<vector<n> > a) { return OP1(Length); }
// normalize
template <int n>
inline Value<vector<n> > normalize(Value<vector<n> > a) { return OP1(Normalize); }

// pow
inline Value<float> pow(Value<float> a, Value<float> b) { return OP2(Pow); }
template <int n>
inline Value<vector<n> > pow(Value<vector<n> > a, Value<float> b) { return OP2(Pow); }
template <int n>
inline Value<vector<n> > pow(Value<vector<n> > a, Value<vector<n> > b) { return OP2(Pow); }

// min
inline Value<float> min(Value<float> a, Value<float> b) { return OP2(Min); }
template <int n>
Value<vector<n> > min(Value<vector<n> > a, Value<vector<n> > b) { return OP2(Min); }
// max
inline Value<float> max(Value<float> a, Value<float> b) { return OP2(Max); }
template <int n>
Value<vector<n> > max(Value<vector<n> > a, Value<vector<n> > b) { return OP2(Max); }

// abs
inline Value<float> abs(Value<float> a) { return OP1(Abs); }

// sin
inline Value<float> sin(Value<float> a) { return OP1(Sin); }
// cos
inline Value<float> cos(Value<float> a) { return OP1(Cos); }

// exp
inline Value<float> exp(Value<float> a) { return OP1(Exp); }
// exp2 (возведение 2 в степень)
inline Value<float> exp2(Value<float> a) { return OP1(Exp2); }
// log
inline Value<float> log(Value<float> a) { return OP1(Log); }

// saturate
inline Value<float> saturate(Value<float> a) { return OP1(Saturate); }
template <int n>
inline Value<vector<n> > saturate(Value<vector<n> > a) { return OP1(Saturate); }

// ddx
inline Value<float> ddx(Value<float> a) { return OP1(Ddx); }
template <int n>
inline Value<vector<n> > ddx(Value<vector<n> > a) { return OP1(Ddx); }
// ddy
inline Value<float> ddy(Value<float> a) { return OP1(Ddy); }
template <int n>
inline Value<vector<n> > ddy(Value<vector<n> > a) { return OP1(Ddy); }

// clip
inline Expression clip(Value<float> a) { return OP1(Clip); }

#undef OP2

END_INANITY_SHADERS

#endif
