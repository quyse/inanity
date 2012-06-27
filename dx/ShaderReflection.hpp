#ifndef ___INANITY_DX_SHADER_REFLECTION_HPP___
#define ___INANITY_DX_SHADER_REFLECTION_HPP___

#include "dx.hpp"
#include "../../ComPointer.hpp"
#include "d3d.hpp"
#include <d3dcompiler.h>
#include <vector>

BEGIN_INANITY

class File;

END_INANITY

BEGIN_INANITY_DX

/// Класс метаинформации о шейдере.
class ShaderConstantBuffer;
class GeometryFormat;
class ShaderLayout;
class System;
class ShaderReflection : public Object
{
private:
	/// Байткод шейдера.
	ptr<File> shader;
	/// Указатель на интерфейс DirectX
	ComPointer<ID3D11ShaderReflection> reflection;
	/// Количество входных параметров шейдера.
	unsigned inputParametersCount;

public:
	/// Создать метаинформацию.
	/** Передаётся скомпилированный шейдер. */
	ShaderReflection(ptr<File> shader);

	/// Создать layout для заданного формата геометрии.
	/** Реализовано только для одного входного слота, то есть одного вершинного буфера. */
	ptr<ShaderLayout> CreateShaderLayout(System* system, GeometryFormat* geometryFormat);
};

END_INANITY_DX

#endif
