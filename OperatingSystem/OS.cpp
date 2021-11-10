#include "OS.h"

int OS::Init()
{
	PCB::process_map["a.exe"] = A_exe::main;

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

	return -1;
}

int OS::CreateProcess()
{
	using std::cin;

	string process_name;
	int need_time;
	cin >> process_name;
	cin >> need_time;
	
	PCB* pcb = nullptr;
	if (PCB::process_map[process_name]) {
		pcb = new PCB(
			process_name, 
			need_time,
			new std::thread(PCB::process_map[process_name],"")
		);
	}
	else {
		printf("* Can't find process \"%s\".\n", process_name.c_str());
		return 1;
	}

	pcb->state = PCB::STATUS::Wait;
	process_queue->AddProcess(pcb);

	return 0;
}

int OS::Running()
{
	SystemData::GetInstance().s_time++;

	process_queue->SortProcessQueue();

	if (!process_queue->process_queue.empty()) {
		cur_process = process_queue->process_queue[0];
		process_queue->process_queue.erase(process_queue->process_queue.begin());
		cur_process->state = PCB::STATUS::Run;

		if (cur_process->process != nullptr) {
			cur_process->process->detach();
		}

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
	else {
		cur_process = nullptr;
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
	printf("\n * fork : Create new process. (fork name need_time)");
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
	}
	return 0;
}

OS::OS() :
	process_queue(new ProcessQueue())
{
}
