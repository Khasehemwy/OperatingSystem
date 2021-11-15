#pragma once
#include "Includes.h"

class MemoryManager
{
public:
	static enum AllocateStrategy { FirstFit = 1, BestFit, WorstFit };

private:
	struct MemoryBlock
	{
		unsigned int address;
		unsigned int size;
	};
	std::list<MemoryBlock> memory_blocks;

	unsigned int max_block_num = 10;
	unsigned int max_block_size = 50;
	std::vector<int> request_blocks;

	AllocateStrategy strategy = AllocateStrategy::FirstFit;

	int RechainBlocks();
	int FirstAllocate();
	int BestAllocate();
	int WorstAllocate();

public:
	int Init();
	int ShowStatus();
	int RequestMemory(int size);
	int ReAllocate(AllocateStrategy strategy);
};