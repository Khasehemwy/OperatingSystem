#pragma once
#include "Includes.h"

class OS
{
	enum Command {
		fork = 1, poweroff, ps,
		help
	};
	ProcessQueue* process_queue;
	PCB* cur_process = nullptr;

	int Init();
	int GetCommand();
	int CreateProcess();
	int Running();
	int DisplayProcess(const PCB* cur_process);
	int CheckProcess();
	int ShowHelp();
public:
	int Boot();

	OS();
};