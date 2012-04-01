#ifndef ___INANITY_DX_CONSTANT_BUFFER_H___
#define ___INANITY_DX_CONSTANT_BUFFER_H___

#include "Buffer.hpp"

BEGIN_INANITY_DX

/// Класс константного буфера.
/** Сделан по сути для того, чтобы константные буферы отличались от остальных графических буферов.
*/
class ConstantBuffer : public Buffer
{
public:
	/// Создать буфер.
	ConstantBuffer(ID3D11Buffer* buffer, size_t size);
};

END_INANITY_DX

#endif
