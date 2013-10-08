#ifndef ___INANITY_OBJECT_POOL_HPP___
#define ___INANITY_OBJECT_POOL_HPP___

#include "ChunkPool.hpp"
#include "PoolObject.hpp"

BEGIN_INANITY

/// Base class for pool of objects.
/** Objects should be inherited from PoolObject. */
class ObjectPoolBase : public ChunkPool
{
	friend class PoolObject;

public:
	ObjectPoolBase(size_t chunkSize) : ChunkPool(chunkSize) {}

	/// Create new object, calling constructor with arguments.
	template <typename T, typename... Args>
	ptr<T> New(Args... args)
	{
		// allocate space for object
		void* chunk = ChunkPool::Allocate();
		// initialize object
		T* object = new (chunk) T(args...);
		// set pool to object
		object->pool = this;

		// return object (wrapping it into ptr)
		return object;
	}

private:
	/// Free object.
	/** Accessible from friend class PoolObject. */
	void Free(PoolObject* object)
	{
		// hold a reference to itself, because
		// object may hold a very last reference
		ptr<ObjectPoolBase> self = this;

		// destroy object
		object->~PoolObject();

		// free object's space
		ChunkPool::Free(object);
	}
};

/// Explicitly typed object pool.
template <typename T>
class ObjectPool : public ObjectPoolBase
{
public:
	ObjectPool() : ObjectPoolBase(sizeof(T)) {}

	template <typename... Args>
	ptr<T> New(Args... args)
	{
		return ObjectPoolBase::New<T, Args...>(args...);
	}
};

END_INANITY

#endif
