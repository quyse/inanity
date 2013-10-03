#include "PoolObject.hpp"
#include "ObjectPool.hpp"

BEGIN_INANITY

void PoolObject::FreeAsNotReferenced()
{
	pool->Free(this);
}

END_INANITY
