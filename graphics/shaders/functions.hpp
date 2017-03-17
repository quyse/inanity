#ifndef ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___
#define ___INANITY_GRAPHICS_SHADERS_FUNCTIONS_HPP___

#include "Value.ipp"
#include "OperationNode.hpp"
#include "ActionNode.hpp"
#include "FragmentNode.hpp"

BEGIN_INANITY_SHADERS

#define OP0(op, ...) ptr<ValueNode>(NEW(OperationNode(OperationNode::operation ## op, __VA_ARGS__)))
#define OP1(op, ...) ptr<ValueNode>(NEW(OperationNode(OperationNode::operation ## op, __VA_ARGS__, a.GetNode())))
#define OP2(op, ...) ptr<ValueNode>(NEW(OperationNode(OperationNode::operation ## op, __VA_ARGS__, a.GetNode(), b.GetNode())))
#define OP3(op, ...) ptr<ValueNode>(NEW(OperationNode(OperationNode::operation ## op, __VA_ARGS__, a.GetNode(), b.GetNode(), c.GetNode())))
#define OP4(op, ...) ptr<ValueNode>(NEW(OperationNode(OperationNode::operation ## op, __VA_ARGS__, a.GetNode(), b.GetNode(), c.GetNode(), d.GetNode())))
#define AC0(ac) ptr<Node>(NEW(ActionNode(ActionNode::action ## ac)))
#define AC1(ac) ptr<Node>(NEW(ActionNode(ActionNode::action ## ac, a.GetNode())))
#define AC2(ac) ptr<Node>(NEW(ActionNode(ActionNode::action ## ac, a.GetNode(), b.GetNode())))

// construction values
inline Value<vec2> newvec2(Value<float> a, Value<float> b) { return OP2(Float11to2, DataTypes::_vec2); }
inline Value<vec3> newvec3(Value<float> a, Value<float> b, Value<float> c) { return OP3(Float111to3, DataTypes::_vec3); }
inline Value<vec4> newvec4(Value<float> a, Value<float> b, Value<float> c, Value<float> d) { return OP4(Float1111to4, DataTypes::_vec4); }
inline Value<vec4> newvec4(Value<vec3> a, Value<float> b) { return OP2(Float31to4, DataTypes::_vec4); }
inline Value<vec4> newvec4(Value<vec2> a, Value<float> b, Value<float> c) { return OP3(Float211to4, DataTypes::_vec4); }
inline Value<ivec2> newivec2(Value<int> a, Value<int> b) { return OP2(Int11to2, DataTypes::_ivec2); }

// constants
inline Value<float> val(float a) { return Value<float>(a); }
inline Value<vec2> val(const vec2& a) { return newvec2(a.x, a.y); }
inline Value<vec3> val(const vec3& a) { return newvec3(a.x, a.y, a.z); }
inline Value<vec4> val(const vec4& a) { return newvec4(a.x, a.y, a.z, a.w); }
inline Value<ivec2> val(const ivec2& a) { return newivec2(a.x, a.y); }

// setPosition
inline Expression setPosition(Value<vec4> a) { return AC1(SetPosition); }
// getInstanceID
inline Value<uint> getInstanceID() { return OP0(GetInstanceID, DataTypes::_uint); }

// output fragment
inline Expression fragment(int target, Value<vec4> f)
{
	return ptr<Node>(NEW(FragmentNode(target, f.GetNode())));
}

// screenToTexture
inline Value<vec2> screenToTexture(Value<vec2> a) { return OP1(ScreenToTexture, DataTypes::_vec2); }

// dot
template <typename T, int n>
inline Value<T> dot(Value<xvec<T, n> > a, Value<xvec<T, n> > b) { return OP2(Dot, DataTypeOf<T>()); }

// cross
template <typename T>
inline Value<xvec<T, 3> > cross(Value<xvec<T, 3> > a, Value<xvec<T, 3> > b) { return OP2(Cross, DataTypeOf<xvec<T, 3> >()); }

// mul
template <typename T, int n, int m, int k>
inline Value<xmat<T, n, k> > mul(Value<xmat<T, n, m> > a, Value<xmat<T, m, k> > b) { return OP2(Mul, DataTypeOf<xmat<T, n, k> >()); }
template <typename T, int n, int m>
inline Value<xvec<T, m> > mul(Value<xvec<T, n> > a, Value<xmat<T, n, m> > b) { return OP2(Mul, DataTypeOf<xvec<T, m> >()); }
template <typename T, int n, int m>
inline Value<xvec<T, n> > mul(Value<xmat<T, n, m> > a, Value<xvec<T, m> > b) { return OP2(Mul, DataTypeOf<xvec<T, n> >()); }

// length
template <typename T, int n>
inline Value<T> length(Value<xvec<T, n> > a) { return OP1(Length, DataTypeOf<T>()); }
// normalize
template <typename T, int n>
inline Value<xvec<T, n> > normalize(Value<xvec<T, n> > a) { return OP1(Normalize, DataTypeOf<xvec<T, n> >()); }

// lerp
template <typename T, int n>
inline Value<xvec<T, n> > lerp(Value<xvec<T, n> > a, Value<xvec<T, n> > b, Value<xvec<T, n> > c) { return OP3(Lerp, DataTypeOf<xvec<T, n> >()); }
template <typename T, int n>
inline Value<xvec<T, n> > lerp(Value<xvec<T, n> > a, Value<xvec<T, n> > b, Value<T> c) { return OP3(Lerp, DataTypeOf<xvec<T, n> >()); }
template <typename T>
inline Value<T> lerp(Value<T> a, Value<T> b, Value<T> c) { return OP3(Lerp, DataTypeOf<T>()); }

// pow
template <typename T>
inline Value<T> pow(Value<T> a, Value<T> b) { return OP2(Pow, DataTypeOf<T>()); }
template <typename T, int n>
inline Value<xvec<T, n> > pow(Value<xvec<T, n> > a, Value<T> b1)
{
	Value<xvec<T, n> > b = b1.template Cast<xvec<T, n> >();
	return OP2(Pow, DataTypeOf<xvec<T, n> >());
}
template <typename T, int n>
inline Value<xvec<T, n> > pow(Value<xvec<T, n> > a, Value<xvec<T, n> > b) { return OP2(Pow, DataTypeOf<xvec<T, n> >()); }

// min
template <typename T>
inline Value<T> min(Value<T> a, Value<T> b) { return OP2(Min, DataTypeOf<T>()); }
// max
template <typename T>
inline Value<T> max(Value<T> a, Value<T> b) { return OP2(Max, DataTypeOf<T>()); }

// abs
template <typename T>
inline Value<T> abs(Value<T> a) { return OP1(Abs, DataTypeOf<T>()); }

// sqrt
inline Value<float> sqrt(Value<float> a) { return OP1(Sqrt, DataTypes::_float); }

// sin
inline Value<float> sin(Value<float> a) { return OP1(Sin, DataTypes::_float); }
// cos
inline Value<float> cos(Value<float> a) { return OP1(Cos, DataTypes::_float); }
// atan2
inline Value<float> atan2(Value<float> a, Value<float> b) { return OP2(Atan2, DataTypes::_float); }

// exp
inline Value<float> exp(Value<float> a) { return OP1(Exp, DataTypes::_float); }
// exp2 (возведение 2 в степень)
inline Value<float> exp2(Value<float> a) { return OP1(Exp2, DataTypes::_float); }
// log
inline Value<float> log(Value<float> a) { return OP1(Log, DataTypes::_float); }

// saturate
inline Value<float> saturate(Value<float> a) { return OP1(Saturate, DataTypes::_float); }
template <typename T, int n>
inline Value<xvec<T, n> > saturate(Value<xvec<T, n> > a) { return OP1(Saturate, DataTypeOf<xvec<T, n> >()); }

// ddx
template <typename T>
inline Value<T> ddx(Value<T> a) { return OP1(Ddx, DataTypeOf<T>()); }
template <typename T, int n>
inline Value<xvec<T, n> > ddx(Value<xvec<T, n> > a) { return OP1(Ddx, DataTypeOf<xvec<T, n> >()); }
// ddy
template <typename T>
inline Value<T> ddy(Value<T> a) { return OP1(Ddy, DataTypeOf<T>()); }
template <typename T, int n>
inline Value<xvec<T, n> > ddy(Value<xvec<T, n> > a) { return OP1(Ddy, DataTypeOf<xvec<T, n> >()); }

// clip
inline Expression clip(Value<float> a) { return AC1(Clip); }

// floor
template <typename T>
inline Value<T> floor(Value<T> a) { return OP1(Floor, DataTypeOf<T>()); }
template <typename T, int n>
inline Value<xvec<T, n> > floor(Value<xvec<T, n> > a) { return OP1(Floor, DataTypeOf<xvec<T, n> >()); }
// ceil
template <typename T>
inline Value<T> ceil(Value<T> a) { return OP1(Ceil, DataTypeOf<T>()); }
template <typename T, int n>
inline Value<xvec<T, n> > ceil(Value<xvec<T, n> > a) { return OP1(Ceil, DataTypeOf<xvec<T, n> >()); }
// mod
inline Value<float> mod(Value<float> a, Value<float> b) { return OP2(Mod, DataTypes::_float); }

#undef OP0
#undef OP1
#undef OP2
#undef OP3
#undef OP4
#undef AC0
#undef AC1
#undef AC2

END_INANITY_SHADERS

#endif
