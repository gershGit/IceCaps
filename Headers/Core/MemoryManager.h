#pragma once
#include <vector>

inline void charAppend(char * destination, char * source, int position, size_t size) {
	memcpy(&destination[position], source, size);
}

inline void charAppend(char * destination, const char * source, int position, size_t size) {
	memcpy(&destination[position], source, size);
}

struct stack {
	void* base;
	void* ptr;
	size_t size;
	size_t allocated;
};

class MemoryManager
{
private:
	std::vector<stack> stacks;
	int stackCount = 0;
public:
	MemoryManager();

	void newStack(size_t stack_size);
	void cleanup();
	void* allocateOnStack(int stackID, size_t allocation_size);

	~MemoryManager();
};

