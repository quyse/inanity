#include "ShaderBuilder.hpp"
#include <sstream>

UniformBuffer::UniformBuffer(int slot, int offset, int size)
: slot(slot), offset(offset), size(size) {}

void ShaderBuilderBase::RegisterUniformBuffer(int slot, int offset, int size)
{
	Shader::RegisterUniformBuffer(slot);
	uniformBuffers.push_back(UniformBuffer(slot, offset, size));
}

Expression ShaderBuilderBase::sampler(int slot)
{
	// проверить, что есть семплер
	if(slot >= (int)samplers.size() || samplers[slot].samplerType == SamplerTypes::_None)
	{
		std::ostringstream s;
		s << "Sampler " << slot << " is not registered";
		THROW_PRIMARY_EXCEPTION(s.str());
	}

	// вернуть семплер
	return NEW(SamplerExpressionObject(slot));
}
