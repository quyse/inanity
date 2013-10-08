#ifndef ___INANITY_POOL_OBJECT_HPP___
#define ___INANITY_POOL_OBJECT_HPP___

#include "RefCounted.hpp"

BEGIN_INANITY

class ObjectPoolBase;

class PoolObject : public RefCounted
{
	friend class ObjectPoolBase;

private:
	ptr<ObjectPoolBase> pool;

protected:
	void FreeAsNotReferenced();
};

END_INANITY

#endif
