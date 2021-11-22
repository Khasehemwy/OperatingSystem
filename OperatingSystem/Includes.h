#pragma once

#include <bits/stdc++.h>
#include <thread>

class SystemData;
class MemoryManager;
class PCB;
class ProcessQueue;
class OS;

#include "SystemData.h"
#include "MemoryManager.h"
#include "PCB.h"
#include "ProcessTable.h"
#include "ProcessQueue.h"
#include "OS.h"


inline float rand_lr(float l, float r) {
	static bool first = true;
	if (first) {
		first = false;
		srand(time(NULL));
	}
	return  l + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (r - l)));
}