#ifndef ___INANITY_SCRIPT_CONVERT_HPP___
#define ___INANITY_SCRIPT_CONVERT_HPP___

#include "script.hpp"

BEGIN_INANITY_SCRIPT

class State;
class Any;

/// Converter of value-type things from script.
template <typename T>
struct ConverterFromScript;

template <typename T>
T ConvertFromScript(ptr<Any> value)
{
	return ConverterFromScript<T>::Convert(value);
}

/// Converter of value-type things to script.
template <typename T>
struct ConverterToScript;

template <typename T>
ptr<Any> ConvertToScript(ptr<State> state, const T& value)
{
	return ConverterToScript<T>::Convert(state, value);
}

END_INANITY_SCRIPT

#endif
