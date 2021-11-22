#pragma once
#include "Includes.h"

class SystemData 
{
	SystemData();
	static SystemData system_data;
public:
	static int s_time;
	static unsigned int s_memory_physical_size;
	static unsigned int s_memory_vitual_size;

	static SystemData GetInstance();
};