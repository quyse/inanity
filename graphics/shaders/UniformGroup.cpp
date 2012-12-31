#include "UniformGroup.hpp"
#include "UniformNode.hpp"
#include "../../MemoryFile.hpp"

UniformGroup::UniformGroup(int slot)
: slot(slot), bufferSize(0), data(0) {}

int UniformGroup::GetSlot() const
{
	return slot;
}

void UniformGroup::Finalize()
{
	dataFile = NEW(MemoryFile(bufferSize));
	data = dataFile->GetData();
}

void* UniformGroup::GetData() const
{
	return data;
}

int UniformGroup::GetSize() const
{
	return bufferSize;
}
