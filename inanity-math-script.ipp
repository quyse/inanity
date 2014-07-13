#include "math/script.ipp"

BEGIN_INANITY_SCRIPT

template Math::xvec<float, 2> ConvertFromScript<META_PROVIDER, Math::xvec<float, 2> >(ptr<Any> value);
template Math::xvec<float, 3> ConvertFromScript<META_PROVIDER, Math::xvec<float, 3> >(ptr<Any> value);
template Math::xvec<float, 4> ConvertFromScript<META_PROVIDER, Math::xvec<float, 4> >(ptr<Any> value);

template ptr<Any> ConvertToScript<META_PROVIDER, Math::xvec<float, 2> >(ptr<State> state, const Math::xvec<float, 2>& value);
template ptr<Any> ConvertToScript<META_PROVIDER, Math::xvec<float, 3> >(ptr<State> state, const Math::xvec<float, 3>& value);
template ptr<Any> ConvertToScript<META_PROVIDER, Math::xvec<float, 4> >(ptr<State> state, const Math::xvec<float, 4>& value);

template Math::xmat<float, 4, 4> ConvertFromScript<META_PROVIDER, Math::xmat<float, 4, 4> >(ptr<Any> value);

template ptr<Any> ConvertToScript<META_PROVIDER, Math::xmat<float, 4, 4> >(ptr<State> state, const Math::xmat<float, 4, 4>& value);

END_INANITY_SCRIPT
