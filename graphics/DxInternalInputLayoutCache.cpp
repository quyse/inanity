#include "DxInternalInputLayoutCache.hpp"
#include "VertexLayout.hpp"
#include "DxVertexShader.hpp"
#include "DxInternalInputLayout.hpp"
#include "DxDevice.hpp"

DxInternalInputLayoutCache::Key::operator size_t() const
{
	return size_t(vertexLayout) ^ size_t(vertexShader);
}

DxInternalInputLayoutCache::DxInternalInputLayoutCache(ptr<DxDevice> device)
: device(device) {}

ptr<DxInternalInputLayout> DxInternalInputLayoutCache::GetInputLayout(VertexLayout* vertexLayout, DxVertexShader* vertexShader)
{
	Key key(vertexLayout, vertexShader);
	InputLayouts::const_iterator i = inputLayouts.find(key);
	if(i != inputLayouts.end())
		return i->second;

	ptr<DxInternalInputLayout> inputLayout = device->CreateInternalInputLayout(vertexLayout, vertexShader);

	inputLayouts[key] = inputLayout;

	return inputLayout;
}
