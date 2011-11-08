#ifndef ___INANITY_GRAPHICS_BUFFER_H___
#define ___INANITY_GRAPHICS_BUFFER_H___

#include "Object.h"
#include "ComPointer.h"
#include "d3d.h"

BEGIN_INANITY

/// Класс графического буфера.
/** Представляет собой буфер заданного размера, который
можно использовать в DirectX в разных целях. */
class GraphicsBuffer : public Object
{
private:
	/// буфер
	ComPointer<ID3D11Buffer> buffer;
	/// Размер буфера.
	size_t size;

public:
	/// Создать буфер.
	GraphicsBuffer(ID3D11Buffer* buffer, size_t size);

	/// Получить интерфейс.
	ID3D11Buffer* GetInternalInterface() const;
	/// Получить размер буфера.
	size_t GetSize() const;
};

END_INANITY

#endif
