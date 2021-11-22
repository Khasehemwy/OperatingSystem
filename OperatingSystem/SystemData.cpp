#include "SystemData.h"

int SystemData::s_time = 0;
unsigned int SystemData::s_memory_physical_size = rand_lr(4, 10);
unsigned int SystemData::s_memory_vitual_size = rand_lr(s_memory_physical_size+1, s_memory_physical_size*2);

SystemData SystemData::GetInstance()
{
	return SystemData(system_data);
}
