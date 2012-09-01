#include "ShaderBuilder.hpp"

UniformBuffer::UniformBuffer(int slot, int offset, int size)
: slot(slot), offset(offset), size(size) {}

void ShaderBuilderBase::uniformBuffer(int slot, int offset, int size)
{
	uniformBuffers.push_back(UniformBuffer(slot, offset, size));
}

Expression ShaderBuilderBase::sampler(int slot)
{
	// зарегистрировать слот семплера
	samplerSlotsMask |= 1 << slot;

	// вернуть семплер
	return NEW(SamplerExpressionObject(slot));
}
