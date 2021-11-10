#pragma once
#pragma warning(disable : 4996)
#include <thread>
#include <mutex>
#include <chrono>
#include <bits/stdc++.h>

namespace A_exe {
	void ReadFile(const char* filePath);

	void PrintInfo();

	void s_get();//将input放入buffer_s

	void s_copy();//将buffer_s放入buffer_t

	void s_put();//将buffer_t放入output

	void s_display();

	int main(const char* args);
};