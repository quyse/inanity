#include "UniformGroup.hpp"
#include "UniformNode.hpp"
#include "../Device.hpp"
#include "../UniformBuffer.hpp"
#include "../ContextState.hpp"
#include "../Context.hpp"
#include "../../MemoryFile.hpp"
#include "../../Exception.hpp"

UniformGroup::UniformGroup(ptr<Device> device, int slot)
: device(device), slot(slot), bufferSize(0), data(0) {}

int UniformGroup::GetSlot() const
{
	return slot;
}

void UniformGroup::Finalize()
{
	dataFile = NEW(MemoryFile(bufferSize));
	data = dataFile->GetData();
	buffer = device->CreateUniformBuffer(bufferSize);
}

void* UniformGroup::GetData() const
{
#ifdef _DEBUG
	if(!dataFile)
		THROW_PRIMARY_EXCEPTION("Uniform group has not finalized");
#endif
	return data;
}

int UniformGroup::GetSize() const
{
	return bufferSize;
}

void UniformGroup::Apply(ContextState& contextState)
{
	contextState.uniformBuffers[slot] = buffer;
}

void UniformGroup::Upload(Context* context)
{
	context->SetUniformBufferData(buffer, data, bufferSize);
}
