#include "PCB.h"

std::map <string, std::function<int(const char*)>> PCB::process_map;

PCB::PCB(string name, int need_time) :
    name(name),
    process(nullptr),
    in_time(SystemData::s_time),
    need_time(need_time),
    used_time(0),
    state(STATUS::Wait),
    priority(std::rand() % MAX_PRIORITY)
{
}

PCB::PCB(string name, int need_time, std::thread* process) :
    name(name),
    process(process),
    in_time(SystemData::s_time),
    need_time(need_time),
    used_time(0),
    state(STATUS::Wait),
    priority(std::rand() % MAX_PRIORITY)
{
}
