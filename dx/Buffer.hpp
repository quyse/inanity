#ifndef ___INANITY_DX_BUFFER_H___
#define ___INANITY_DX_BUFFER_H___

#include "dx.hpp"
#include "../../ComPointer.hpp"
#include "d3d.hpp"

BEGIN_INANITY_DX

/// Класс буфера.
/** Представляет собой буфер заданного размера, который
можно использовать в DirectX в разных целях. */
class Buffer : public Object
{
private:
	/// буфер
	ComPointer<ID3D11Buffer> buffer;
	/// Размер буфера.
	size_t size;

public:
	/// Создать буфер.
	Buffer(ID3D11Buffer* buffer, size_t size);

	/// Получить интерфейс.
	ID3D11Buffer* GetInternalInterface() const;
	/// Получить размер буфера.
	size_t GetSize() const;
};

END_INANITY_DX

#endif
