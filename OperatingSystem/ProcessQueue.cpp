#include "ProcessQueue.h"

int ProcessQueue::AddProcess(PCB* process)
{
	process_queue.push_back(process);

	return 0;
}

int ProcessQueue::SortProcessQueue()
{
	std::sort(process_queue.begin(), process_queue.end(), [=](const PCB* a, const PCB* b)->bool {
		return a->priority > b->priority;
	});
	return 0;
}
