#ifndef ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___

#include "Value.hpp"
#include "OperationNode.hpp"
#include "FragmentNode.hpp"

BEGIN_INANITY_SHADERS

#define OP0(op) ptr<Node>(NEW(OperationNode(OperationNode::operation ## op)))
#define OP1(op) ptr<Node>(NEW(OperationNode(OperationNode::operation ## op, a.GetNode())))
#define OP2(op) ptr<Node>(NEW(OperationNode(OperationNode::operation ## op, a.GetNode(), b.GetNode())))
#define OP3(op) ptr<Node>(NEW(OperationNode(OperationNode::operation ## op, a.GetNode(), b.GetNode(), c.GetNode())))
#define OP4(op) ptr<Node>(NEW(OperationNode(OperationNode::operation ## op, a.GetNode(), b.GetNode(), c.GetNode(), d.GetNode())))

// construction values
inline Value<vec2> newvec2(Value<float> a, Value<float> b) { return OP2(Float11to2); }
inline Value<vec3> newvec3(Value<float> a, Value<float> b, Value<float> c) { return OP3(Float111to3); }
inline Value<vec4> newvec4(Value<float> a, Value<float> b, Value<float> c, Value<float> d) { return OP4(Float1111to4); }
inline Value<vec4> newvec4(Value<vec3> a, Value<float> b) { return OP2(Float31to4); }
inline Value<vec4> newvec4(Value<vec2> a, Value<float> b, Value<float> c) { return OP3(Float211to4); }

// setPosition
inline Expression setPosition(Value<vec4> a) { return OP1(SetPosition); }
// getInstanceID
inline Value<uint> getInstanceID() { return OP0(GetInstanceID); }

// output fragment
inline Expression fragment(int target, Value<vec4> f)
{
	return ptr<Node>(NEW(FragmentNode(target, f.GetNode())));
}

// screenToTexture
inline Value<vec2> screenToTexture(Value<vec2> a) { return OP1(ScreenToTexture); }

// dot
template <typename T, int n>
inline Value<T> dot(Value<xvec<T, n> > a, Value<xvec<T, n> > b) { return OP2(Dot); }

// cross
template <typename T>
inline Value<xvec<T, 3> > cross(Value<xvec<T, 3> > a, Value<xvec<T, 3> > b) { return OP2(Cross); }

// mul
template <typename T, int n, int m, int k>
inline Value<xmat<T, n, k> > mul(Value<xmat<T, n, m> > a, Value<xmat<T, m, k> > b) { return OP2(Mul); }
template <typename T, int n, int m>
inline Value<xvec<T, m> > mul(Value<xvec<T, n> > a, Value<xmat<T, n, m> > b) { return OP2(Mul); }
template <typename T, int n, int m>
inline Value<xvec<T, n> > mul(Value<xmat<T, n, m> > a, Value<xvec<T, m> > b) { return OP2(Mul); }

// length
template <typename T, int n>
inline Value<T> length(Value<xvec<T, n> > a) { return OP1(Length); }
// normalize
template <typename T, int n>
inline Value<xvec<T, n> > normalize(Value<xvec<T, n> > a) { return OP1(Normalize); }

// lerp
template <typename T, int n>
inline Value<xvec<T, n> > lerp(Value<xvec<T, n> > a, Value<xvec<T, n> > b, Value<xvec<T, n> > c) { return OP3(Lerp); }
template <typename T, int n>
inline Value<xvec<T, n> > lerp(Value<xvec<T, n> > a, Value<xvec<T, n> > b, Value<T> c) { return OP3(Lerp); }
template <typename T>
inline Value<T> lerp(Value<T> a, Value<T> b, Value<T> c) { return OP3(Lerp); }

// pow
template <typename T>
inline Value<T> pow(Value<T> a, Value<T> b) { return OP2(Pow); }
template <typename T, int n>
inline Value<xvec<T, n> > pow(Value<xvec<T, n> > a, Value<T> b1)
{
	Value<xvec<T, n> > b = b1.template Cast<xvec<T, n> >();
	return OP2(Pow);
}
template <typename T, int n>
inline Value<xvec<T, n> > pow(Value<xvec<T, n> > a, Value<xvec<T, n> > b) { return OP2(Pow); }

// min
template <typename T>
inline Value<T> min(Value<T> a, Value<T> b) { return OP2(Min); }
// max
template <typename T>
inline Value<T> max(Value<T> a, Value<T> b) { return OP2(Max); }

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
template <typename T, int n>
inline Value<xvec<T, n> > saturate(Value<xvec<T, n> > a) { return OP1(Saturate); }

// ddx
inline Value<float> ddx(Value<float> a) { return OP1(Ddx); }
template <typename T, int n>
inline Value<xvec<T, n> > ddx(Value<xvec<T, n> > a) { return OP1(Ddx); }
// ddy
inline Value<float> ddy(Value<float> a) { return OP1(Ddy); }
template <typename T, int n>
inline Value<xvec<T, n> > ddy(Value<xvec<T, n> > a) { return OP1(Ddy); }

// clip
inline Expression clip(Value<float> a) { return OP1(Clip); }

#undef OP2

END_INANITY_SHADERS

#endif
