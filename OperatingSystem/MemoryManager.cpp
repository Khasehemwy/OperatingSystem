#include "MemoryManager.h"

int MemoryManager::RechainBlocks()
{
	auto block_it = memory_blocks.begin();
	for (; block_it != memory_blocks.end(); block_it++) {
		if (block_it->size <= 0) { memory_blocks.erase(block_it++); }
	}

	return 0;
}

int MemoryManager::FirstAllocate()
{
	//首次找到就分配
	auto request_block = request_blocks.begin();
	for (; request_block != request_blocks.end(); request_block++) {
		bool is_allocated = false;
		for (auto& block : memory_blocks) {
			if (block.size >= *request_block) {
				block.address += *request_block;
				block.size -= *request_block;
				RechainBlocks();
				is_allocated = true;
				break;
			}
		}

		if (!is_allocated) {
			request_blocks.erase(request_block++);
			printf("[Error][Memory]: Allocate failed.\n");
			return 1;
		}
	}
	return 0;
}

int MemoryManager::BestAllocate()
{
	//寻找能够分配并且大小最接近需要的
	auto request_block = request_blocks.begin();
	for (; request_block != request_blocks.end(); request_block++) {
		bool is_allocated = false;
		MemoryBlock* best_block = nullptr;
		int best_block_size = -1;
		for (auto& block : memory_blocks) {
			if (block.size >= *request_block) {
				if (best_block_size == -1 || block.size < best_block_size) {
					best_block = &block;
				}
			}
		}
		if (best_block != nullptr) {
			best_block->address += *request_block;
			best_block->size -= *request_block;
			RechainBlocks();
			is_allocated = true;
		}

		if (!is_allocated) {
			request_blocks.erase(request_block++);
			printf("[Error][Memory]: Allocate failed.\n");
			return 1;
		}
	}

	return 0;
}

int MemoryManager::WorstAllocate()
{
	//寻找能够分配并且大小最大的
	auto request_block = request_blocks.begin();
	for (; request_block != request_blocks.end(); request_block++) {
		bool is_allocated = false;
		MemoryBlock* worst_block = nullptr;
		int worst_block_size = -1;
		for (auto& block : memory_blocks) {
			if (block.size >= *request_block) {
				if (block.size > worst_block_size) {
					worst_block = &block;
				}
			}
		}
		if (worst_block != nullptr) {
			worst_block->address += *request_block;
			worst_block->size -= *request_block;
			RechainBlocks();
			is_allocated = true;
		}

		if (!is_allocated) {
			request_blocks.erase(request_block++);
			printf("[Error][Memory]: Allocate failed.\n");
			return 1;
		}
	}

	return 0;
}

int MemoryManager::Init()
{
	memory_blocks.clear();
	request_blocks.clear();
	int block_size = rand_lr(1, max_block_num);
	int address = 20;
	for (int i = 0; i < block_size; i++) {
		int size = rand_lr(1, max_block_size);
		MemoryBlock memory_block = { address,size };
		memory_blocks.push_back(memory_block);
		address += size;
	}

	return 0;
}

int MemoryManager::ShowStatus()
{
	printf("\n空闲区始址（k) \t 空闲区大小（k）\n");
	for (auto& block : memory_blocks) {
		printf("%d \t %d \n", block.address, block.size);
	}
	printf("\n");

	return 0;
}

int MemoryManager::ReAllocate(AllocateStrategy strategy)
{
	this->strategy = strategy;
	if (strategy == AllocateStrategy::FirstFit) {
		return this->FirstAllocate();
	}
	else if (strategy == AllocateStrategy::BestFit) {
		return this->BestAllocate();
	}
	else if (strategy == AllocateStrategy::WorstFit) {
		return this->WorstAllocate();
	}
	return 0;
}

int MemoryManager::RequestMemory(int size)
{
	if (request_blocks.size() < max_block_num) {
		request_blocks.push_back(size);

		memory_blocks.clear();
		int block_size = rand_lr(1, max_block_num);
		int address = 20;
		for (int i = 0; i < block_size; i++) {
			int size = rand_lr(1, max_block_size);
			MemoryBlock memory_block = { address,size };
			memory_blocks.push_back(memory_block);
			address += size;
		}

		return ReAllocate(strategy);
	}
	else {
		printf("[Error][Memory]: Arrive at the maximum work distribution.\n");
		return 1;
	}
	return 0;
}
