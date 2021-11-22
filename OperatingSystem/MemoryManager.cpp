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
		if (request_block->allocated) { continue; }
		bool is_allocated = false;
		for (auto& block : memory_blocks) {
			if (block.size >= request_block->size) {
				block.address += request_block->size;
				block.size -= request_block->size;
				request_block->allocated = true;
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
		if (request_block->allocated) { continue; }
		bool is_allocated = false;
		MemoryBlock* best_block = nullptr;
		int best_block_size = -1;
		for (auto& block : memory_blocks) {
			if (block.size >= request_block->size) {
				if (best_block_size == -1 || block.size < best_block_size) {
					best_block = &block;
				}
			}
		}
		if (best_block != nullptr) {
			best_block->address += request_block->size;
			best_block->size -= request_block->size;
			request_block->allocated = true;
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
		if (request_block->allocated) { continue; }
		bool is_allocated = false;
		MemoryBlock* worst_block = nullptr;
		int worst_block_size = -1;
		for (auto& block : memory_blocks) {
			if (block.size >= request_block->size) {
				if (block.size > worst_block_size) {
					worst_block = &block;
				}
			}
		}
		if (worst_block != nullptr) {
			worst_block->address += request_block->size;
			worst_block->size -= request_block->size;
			request_block->allocated = true;
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
	//分区
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

	//分页
	vitual_memory.clear();


	return 0;
}

int MemoryManager::ShowStatus()
{
	printf("---------------------------------\n");
	printf("\n空闲区始址(k) \t 空闲区大小(k) \n");
	for (auto& block : memory_blocks) {
		printf("%d \t\t %d \n", block.address, block.size);
	}

	printf("\n内存页载入情况\n");
	for (auto& page : vitual_memory) {
		printf("Page id [%d] allocated.\n", page.id);
	}
	printf("---------------------------------\n");
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

int MemoryManager::RequestPage(int request_page_id)
{
	unsigned int page_size = SystemData::GetInstance().s_memory_physical_size;

	for (auto& page : vitual_memory) {
		if (page.id == request_page_id) {
			printf("Request_page_id %d already allocated.\n", request_page_id);
			printf("%d 号页面已分配, 无需中断.\n", request_page_id);
			return 0;
		}
	}
	if (vitual_memory.size() < page_size) {
		vitual_memory.push_back(Page(request_page_id));
		printf("[Interrupt] Allocate page directly.\n");
		printf("[中断] 直接分配页, 无需调出已有页.\n");
	}
	else {
		auto it = vitual_memory.begin();
		vitual_memory.push_back(Page(request_page_id));
		printf("[Interrupt] Transfer out %d page, transfer in %d page\n", it->id, request_page_id);
		printf("[中断] 调出第 %d 页, 调入第 %d 页\n", it->id, request_page_id);
	}

	return 0;
}

int MemoryManager::RequestMemory(int size)
{
	//先申请页
	int request_page_id = rand_lr(0, SystemData::GetInstance().s_memory_vitual_size);
	if (int jug = this->RequestPage(request_page_id)) {
		return jug;
	}
	
	//申请分区
	if (request_blocks.size() < max_block_num) {
		request_blocks.push_back(RequestBlock(size));

		return ReAllocate(strategy);
	}
	else {
		printf("[Error][Memory]: Arrive at the maximum work distribution.\n");
		return 1;
	}
	return 0;
}
