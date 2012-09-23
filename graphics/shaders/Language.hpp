#ifndef ___INANITY_GRAPHICS_SHADERS_LANGUAGE_HPP___
#define ___INANITY_GRAPHICS_SHADERS_LANGUAGE_HPP___

#include "ShaderBuilder.hpp"
#include "Semantic.hpp"
#include "Functions.hpp"
#include "StatementObject.hpp"

/*
Файл, который предполагается вставлять туда, где описываются шейдеры.
И только туда, так как он определяет макросы с короткими именами,
и using namespace.
*/

BEGIN_INANITY_SHADERS

/// Начало шейдера.
#define SHADER_BEGIN(InputType, OutputType, UniformsType, TempType) { \
	ShaderBuilder<InputType, OutputType, UniformsType, TempType> shaderBuilder; \
	struct ShaderTypes { \
		typedef InputType InputStruct; \
		typedef OutputType OutputStruct; \
		typedef UniformsType UniformsStruct; \
		typedef TempType TempStruct; \
	};

/// Указать константный буфер.
#define SHADER_UNIFORM_BUFFER(slot, bufferField) \
	shaderBuilder.uniformBuffer(slot, &ShaderTypes::UniformsStruct::bufferField)

/// Входная семантика.
#define INPUT_SEMANTIC(name, semanticName) IN(name); shaderBuilder.semantic(&ShaderTypes::InputStruct::name, Semantics::semanticName)
/// Выходная семантика.
#define OUTPUT_SEMANTIC(name, semanticName) OUT(name); shaderBuilder.semantic(&ShaderTypes::OutputStruct::name, Semantics::semanticName)

/// Начало кода шейдера.
#define SHADER_CODE_BEGIN() shaderBuilder.SetCode((

/// Входная переменная.
#define IN(name) shaderBuilder.in(&ShaderTypes::InputStruct::name)
/// Выходная переменная.
#define OUT(name) shaderBuilder.out(&ShaderTypes::OutputStruct::name)
/// Uniform-переменная.
//#define UNIFORM(buffer, name) shaderBuilder.uniform(&ShaderTypes::UniformsStruct::buffer::name)
#define UNIFORM(bufferNumber, name) shaderBuilder.uniform(bufferNumber, &name)
/// Временная переменная.
#define TEMP(name) shaderBuilder.temp(&ShaderTypes::TempStruct::name)
/// Семплер.
#define SAMPLER(slot) shaderBuilder.sampler(slot)

/// Выбор компонентов вектора.
#define SWIZZLE(expr, components) (expr).Swizzle(#components)
/// Константа float.
#define CFLOAT(value) MakePointer<ExpressionObject>(NEW(ConstFloatExpressionObject(value)))
/// Константа int.
#define CINT(value) MakePointer<ExpressionObject>(NEW(ConstIntExpressionObject(value)))

/// Конец кода шейдера.
#define SHADER_CODE_END() ))

/// Конец шейдера.
#define SHADER_END() }

END_INANITY_SHADERS

using namespace Inanity::Shaders;

#endif
