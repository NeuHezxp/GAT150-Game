#include "memory.h"
#include <iostream>
using namespace std;
kiko::MemoryTracker  kiko::g_memoryTracker;

void* operator new (size_t size)
{
	void* p = malloc(size);
	kiko::g_memoryTracker.Add(p, size);

	return p;
}

void operator delete (void* address, size_t size) //takes in the pointer and the size to delete it
{
	kiko::g_memoryTracker.Remove(address, size);
	free(address);
}

namespace kiko
{
	void MemoryTracker::Add(void* address, size_t size)
	{
		m_bytesAllocated += size;
		m_numAllocations++;
	}

	void MemoryTracker::Remove(void* address, size_t size)
	{
		m_bytesAllocated -= size;
		m_numAllocations--;
	}

	bool MemoryTracker::Initialize()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		return true;
	}

	void MemoryTracker::DisplayInfo()
	{
		_CrtMemDumpAllObjectsSince(NULL);
	}
}