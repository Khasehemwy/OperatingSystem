#pragma once
#include "Includes.h"

class ProcessQueue
{
public:
    std::vector<PCB*> process_queue;
    int AddProcess(PCB* process);

    int SortProcessQueue();
};