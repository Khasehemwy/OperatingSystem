#pragma once
#pragma warning(disable : 4996)
#include <thread>
#include <mutex>
#include <chrono>
#include <bits/stdc++.h>

namespace A_exe {
	void ReadFile(const char* filePath);

	void PrintInfo();

	void s_get();//��input����buffer_s

	void s_copy();//��buffer_s����buffer_t

	void s_put();//��buffer_t����output

	void s_display();

	int main(const char* args);
};