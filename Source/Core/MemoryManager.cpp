#include "Core/MemoryManager.h"

MemoryManager::MemoryManager()
{
}

void MemoryManager::newStack(size_t stack_size)
{
	stacks.push_back(stack());
	stacks[stackCount].base = malloc(stack_size);
	stacks[stackCount].ptr = stacks[stackCount].base;
	stacks[stackCount].size = stack_size;
	stacks[stackCount].allocated = 0;
	stackCount++;
}

void MemoryManager::cleanup()
{
	for (stack  mStack : stacks ) {
		free(mStack.base);
	}
}

void * MemoryManager::allocateOnStack(int stackID, size_t allocation_size)
{
	void* retVal = stacks[stackID].ptr;
	stacks[stackID].allocated += allocation_size;
	if (stacks[stackID].allocated > stacks[stackID].allocated) {
		return nullptr;
	}
	stacks[stackID].ptr = (void*)((char*)stacks[stackID].ptr + allocation_size);
	return retVal;
}

MemoryManager::~MemoryManager()
{
}
