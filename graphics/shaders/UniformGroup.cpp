#include "UniformGroup.hpp"
#include "UniformNode.hpp"
#include "../Device.hpp"
#include "../UniformBuffer.hpp"
#include "../Context.hpp"
#include "../VertexShader.hpp"
#include "../PixelShader.hpp"
#include "../IndexBuffer.hpp"
#include "../BlendState.hpp"
#include "../../MemoryFile.hpp"
#include "../../Exception.hpp"

BEGIN_INANITY_SHADERS

UniformGroup::UniformGroup(int slot)
: slot(slot), bufferSize(0), data(0) {}

int UniformGroup::GetSlot() const
{
	return slot;
}

ptr<UniformBuffer> UniformGroup::GetBuffer() const
{
	return buffer;
}

void UniformGroup::Finalize(ptr<Device> device)
{
	dataFile = NEW(MemoryFile(bufferSize));
	data = dataFile->GetData();
	buffer = device->CreateUniformBuffer(bufferSize);
}

void* UniformGroup::GetData() const
{
#ifdef _DEBUG
	if(!dataFile)
		THROW("Uniform group has not finalized");
#endif
	return data;
}

int UniformGroup::GetSize() const
{
	return bufferSize;
}

void UniformGroup::Upload(Context* context)
{
	context->UploadUniformBufferData(buffer, data, bufferSize);
}

END_INANITY_SHADERS
