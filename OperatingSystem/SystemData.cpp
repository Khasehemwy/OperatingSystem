#include "SystemData.h"

int SystemData::s_time = 0;

SystemData SystemData::GetInstance()
{
	return SystemData(system_data);
}
