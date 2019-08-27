#pragma once

#include <Core\Definitions.h>
#include <cstdlib>

template<unsigned B>
class PoolAllocatorImpl
{
	struct PoolObject
	{
		PoolObject* next;
	};

public:
	PoolAllocatorImpl(u32 maxObjects, bool allowExtraAllocations = true)
	{
		STATIC_ASSERT(B > sizeof(void *), "PoolAllocatorImpl is created with size B too small!");

		m_sizePerObject = B;
		m_maxObjects = maxObjects;
		m_poolSize = m_maxObjects * m_sizePerObject;
		m_numObjects = 0;
		m_allowExtraAllocations = allowExtraAllocations;
		m_numExtraAllocations = 0;
		m_freeList = nullptr;

		CreatePool();
	}

	~PoolAllocatorImpl()
	{
		ASSERT(m_numExtraAllocations == 0);
		if (m_pool)
		{
			free(m_pool);
			m_pool = nullptr;
		}
	}

	void* Alloc()
	{
		ASSERT(m_pool);
		if (m_freeList == nullptr)
		{
			ASSERT(m_allowExtraAllocations);
			++m_numExtraAllocations;
			++m_numObjects;
			void* object = malloc(m_sizePerObject);
			return object;
		}
		else
		{
			PoolObject* object = m_freeList;
			m_freeList = m_freeList->next;
			++m_numObjects;
			return object;
		}
	}

	void Free(void* object)
	{
		ASSERT(object && m_pool);
		s8* obj = (s8 *)object;
		s64 objDistance, poolDistance;
		objDistance = obj - m_pool;
		poolDistance = (m_pool + m_poolSize) - m_pool;

		if (poolDistance < objDistance && objDistance >= 0)
		{
			ASSERT(m_allowExtraAllocations);
			--m_numExtraAllocations;
			free(object);
		}
		else
		{
			PoolObject* memObj = (PoolObject *)(object);
			memObj->next = m_freeList;
			m_freeList = memObj;
		}

		--m_numObjects;
	}

	void Clear()
	{
		ASSERT(m_numExtraAllocations == 0);
		free(m_pool);

		m_freeList = nullptr;

		CreatePool();
	}

private:

	void CreatePool()
	{
		m_pool = (s8*)malloc(m_poolSize);
		ASSERT(m_pool);

		// init the free list
		for (u32 i = 0; i < m_maxObjects; ++i)
		{
			PoolObject* obj = (PoolObject *)(m_pool + i * m_sizePerObject);
			obj->next = m_freeList;
			m_freeList = obj;
		}
	}

	u32 m_sizePerObject;
	u32 m_maxObjects;
	u32 m_numObjects;
	u32 m_poolSize;
	s8* m_pool;
	bool m_allowExtraAllocations;
	u32 m_numExtraAllocations;
	PoolObject* m_freeList;
};

template<typename T>
class PoolAllocator
{
public:
	PoolAllocator(u32 maxObjects, bool allowExtraAllocations = true) 
		: m_allocator(maxObjects, allowExtraAllocations) {}

	T* Alloc()
	{
		return reinterpret_cast<T *>(m_allocator.Alloc());
	}

	template<typename... Args>
	T* Construct(Args... args)
	{
		T* obj = Alloc();
		T* mem = new (obj) T(args...);
		return obj;
	}

	void Free(T* address)
	{
		address->~T();
		m_allocator.Free(address);
	}

	void Clear()
	{
		m_allocator.Clear();
	}

private:
	PoolAllocatorImpl<sizeof(T)> m_allocator;
};
