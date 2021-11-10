#pragma once
#include "Includes.h"

class SystemData 
{
	SystemData();
	static SystemData system_data;
public:
	static int s_time;

	static SystemData GetInstance();
};