#pragma once
#include "Includes.h"

using std::string;
class PCB
{
    const int MAX_PRIORITY = 30;
public:
    static std::map <string, std::function<int(const char*)>> process_map;
    static enum STATUS { Wait = 1, Run, Finish };

    string name;
    std::thread* process;
    int in_time;
    int need_time;
    int used_time;
    int state;
    int priority;

    int need_memory;

    PCB(string name, int need_time, std::thread* process, int need_memory);
};