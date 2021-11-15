#include "OS.h"

int OS::Init()
{
	memory_manager->Init();

	PCB::process_map["a.exe"] = A_exe::main;
	PCB::process_map["b.exe"] = B_memory_exe::main;

	return 0;
}

int OS::GetCommand()
{
	using std::cin;
	using std::string;

	string command;
	cin >> command;

	if (command == "fork") {
		return Command::fork;
	}
	else if (command == "poweroff") {
		return Command::poweroff;
	}
	else if (command == "ps") {
		return Command::ps;
	}
	else if (command == "help") {
		return Command::help;
	}
	else if (command == "free") {
		return Command::free;
	}

	return -1;
}

int OS::NoCommand()
{
	printf("[Error] No Command. Input \"help\" for more information.\n");
	return 0;
}

int OS::CreateProcess()
{
	using std::cin;

	string process_name;
	int need_time, need_memory;
	cin >> process_name;
	cin >> need_time;
	cin >> need_memory;
	
	PCB* pcb = nullptr;
	if (PCB::process_map[process_name]) {
		if (!memory_manager->RequestMemory(need_memory)) {
			pcb = new PCB(
				process_name, 
				need_time,
				new std::thread(PCB::process_map[process_name],""),
				need_memory
			);
		}
		else {
			printf("[Error][Memory]: Request Memory fail.\n");
		}
	}
	else {
		printf("[Error][Process]: Can't find process \"%s\".\n", process_name.c_str());
		return 1;
	}

	if (pcb == nullptr) {
		printf("[Error][Process]: Create process fail.\n");
		return -1;
	}

	pcb->state = PCB::STATUS::Wait;
	process_queue->AddProcess(pcb);

	return 0;
}

int OS::Running()
{
	SystemData::GetInstance().s_time++;

	process_queue->SortProcessQueue();

	cur_process = nullptr;
	if (!process_queue->process_queue.empty()) {
		cur_process = process_queue->process_queue[0];
		process_queue->process_queue.erase(process_queue->process_queue.begin());

		if (cur_process->process != nullptr && cur_process->state == PCB::STATUS::Wait) {
			cur_process->process->detach();
		}
		cur_process->state = PCB::STATUS::Run;

		cur_process->used_time++;
		if (cur_process->used_time >= cur_process->need_time) {
			printf("\n*the process [%s] completed.*\n", cur_process->name.c_str());
			cur_process->state = PCB::STATUS::Finish;
		}
		else {
			cur_process->priority--;
			process_queue->process_queue.push_back(cur_process);
		}
	}

	if (cur_process == nullptr) {
		printf("No process running.\n");
		return 1;
	}


	return 0;
}

int OS::DisplayProcess(const PCB* cur_process)
{
	printf("|%s\t", cur_process->name.c_str());
	printf("|%d\t", cur_process->state);
	printf("|%d\t", cur_process->priority);
	printf("|%d\t", cur_process->need_time);
	printf("|%d\t", cur_process->used_time);
	printf("\n");

	return 0;
}

int OS::CheckProcess()
{
	if (!process_queue->process_queue.empty()) {
		if (cur_process) {
			printf("\n **** running process is: %s", cur_process->name.c_str());
			printf("\n qname \t state \t super \t ndtime \t runtime \n");
			DisplayProcess(cur_process);
		}

		printf("\n **** waiting processes are:"); /*显示当前就绪进程*/
		printf("\n qname \t state \t super \t ndtime \t runtime \n");
		for (int i = 0; i < process_queue->process_queue.size(); i++) {
			if (cur_process && process_queue->process_queue[i]->name != cur_process->name) {
				DisplayProcess(process_queue->process_queue[i]);
			}
		}
	}
	return 0;
}

int OS::ShowHelp()
{
	printf("\n **** help document ****");
	printf("\n * fork : Create new process. (fork name need_time need_memory)");
	printf("\n * poweroff : poweroff OS.");
	printf("\n * ps : check processes.");
	printf("\n\n");

	return 0;
}

int OS::Boot()
{
	Init();

	printf("Input \"help\" for more information.\n");
	int quit = 0;
	while (!quit)
	{
		Running();
		printf("> ");
		int command = GetCommand();
		if (command == Command::fork) {
			CreateProcess();
		}
		else if (command == Command::poweroff) {
			quit = 1;
		}
		else if (command == Command::ps) {
			CheckProcess();
		}
		else if (command == Command::help) {
			ShowHelp();
		}		
		else if (command == Command::free) {
			memory_manager->ShowStatus();
		}
		else {
			NoCommand();
		}
	}
	return 0;
}

OS::OS() :
	process_queue(new ProcessQueue()),
	memory_manager(new MemoryManager())
{
}
