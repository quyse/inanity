#ifndef ___INANITY_GRAPHICS_SHADERS_LANGUAGE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_LANGUAGE_HPP___

#include "ShaderBuilder.hpp"

/*
Файл, который предполагается вставлять туда, где описываются шейдеры.
И только туда, так как он определяет макросы с короткими именами.
*/

BEGIN_INANITY_SHADERS

/// Начало шейдера.
#define SHADER_BEGIN(InputType, OutputType, UniformsType, TempType) { \
	ShaderBuilder<InputType, OutputType, UniformsType, TempType> shaderBuilder;

/// Указать константный буфер.
#define SHADER_UNIFORM_BUFFER(slot, bufferField) \
	shaderBuilder.uniformBuffer(slot, &decltype(shaderBuilder)::UniformsStruct::bufferField)

/// Начало кода шейдера.
#define SHADER_CODE_BEGIN() shaderBuilder.SetCode((

/// Входная переменная.
#define IN(name) shaderBuilder.in(&decltype(shaderBuilder)::InputStruct::name)
/// Выходная переменная.
#define OUT(name) shaderBuilder.out(&decltype(shaderBuilder)::OutputStruct::name)
/// Uniform-переменная.
#define UNIFORM(buffer, name) shaderBuilder.uniform(&decltype(shaderBuilder)::UniformsStruct::buffer::name)
/// Временная переменная.
#define TEMP(name) shaderBuilder.temp(&decltype(shaderBuilder)::TempStruct::name)
/// Семплер.
#define SAMPLER(slot) shaderBuilder.sampler(slot)

/// Конец кода шейдера.
#define SHADER_CODE_END() ))

/// Конец шейдера.
#define SHADER_END() }

END_INANITY_SHADERS

#endif
