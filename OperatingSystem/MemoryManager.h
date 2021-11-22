#pragma once
#include "Includes.h"

class MemoryManager
{
public:
	static enum AllocateStrategy { FirstFit = 1, BestFit, WorstFit };

private:
	//·ÖÇø
	struct MemoryBlock
	{
		unsigned int address;
		unsigned int size;
	};
	std::list<MemoryBlock> memory_blocks;

	unsigned int max_block_num = 10;
	unsigned int max_block_size = 50;

	struct RequestBlock {
		int size;
		bool allocated;
		RequestBlock(int size) :size(size), allocated(false) {}
	};
	std::vector<RequestBlock> request_blocks;

	AllocateStrategy strategy = AllocateStrategy::FirstFit;

	int RechainBlocks();
	int FirstAllocate();
	int BestAllocate();
	int WorstAllocate();

	//·ÖÒ³
	struct Page
	{
		int id;
		Page(int id) :id(id) { }
	};

	std::list<Page>vitual_memory;
	int RequestPage(int request_page_id);

public:
	int Init();
	int ShowStatus();
	int RequestMemory(int size);
	int ReAllocate(AllocateStrategy strategy);
};