#include <thread>
#include <mutex>
#include <chrono>
#include <bits/stdc++.h>

const char* input;
char output[1000];

unsigned int input_index = 0;
unsigned int input_n = 0;
unsigned int output_index = 0;

const char* c_thread;
int c_runtime, c_lefttime, c_priority;

char buffer_s = NULL;
char buffer_t = NULL;

using std::mutex;
mutex lock;
bool s_flag = false;
bool s_put_flag = false;

void ReadFile(const char* filePath)
{
	std::string fileStr;
	const char* fileChar;
	std::ifstream file(filePath);
	std::ostringstream stream;
	char ch;
	while (stream && file.get(ch)) { stream.put(ch); }
	file.close();
	fileStr = stream.str();
	input = fileStr.c_str();
	input_n = fileStr.size();
}

void PrintInfo()
{
	printf("|  当前进程 |   已运行时间 |   还需要剩余时间 |  进程优先级  |\n");
	printf("|   %s          %d             %d                  %d   \n", c_thread, c_runtime, c_lefttime, c_priority);
}

void s_get()//将input放入buffer_s
{
	int get_run_time = 0;
	int get_left_time = 10;
	int get_priority = 1;
	while (s_flag) {
		lock.lock();

		get_run_time += 1;
		get_left_time -= 1;
		c_thread = "s_get";
		c_runtime = get_run_time;
		c_lefttime = get_left_time;
		c_priority = get_priority;

		if (buffer_s == NULL) {
			buffer_s = input[input_index++];
			printf("|-----------------------------S_GET--------------------------|\n");
			printf("|input_index = %d ; buffer_s = %c        \n", input_index, buffer_s);
			PrintInfo();
			printf("|-----------------------------S_GET--------------------------|\n");
			printf("\n\n\n\n");
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		lock.unlock();
	}
}

void s_copy()//将buffer_s放入buffer_t
{
	int copy_run_time = 0;
	int copy_left_time = 10;
	int copy_priority = 2;

	while (s_flag) {
		lock.lock();

		copy_run_time++;
		copy_left_time--;
		c_thread = "s_copy";
		c_runtime = copy_run_time;
		c_lefttime = copy_left_time;
		c_priority = copy_priority;

		if (buffer_s != NULL) {
			if (buffer_t == NULL) {
				buffer_t = buffer_s;
				buffer_s = NULL;
				printf("|-----------------------------S_COPY-------------------------|\n");
				printf("|             copying from buffer_s to buffer_t......... \n");
				PrintInfo();
				printf("|-----------------------------S_COPY-------------------------|\n");
				printf("\n\n\n\n");
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		}

		lock.unlock();
	}
}

void s_put()//将buffer_t放入output
{
	int put_run_time = 0;
	int put_left_time = 10;
	int put_priority = 3;

	while (s_flag) {
		lock.lock();

		put_run_time++;
		put_left_time--;
		c_thread = "s_put";
		c_runtime = put_run_time;
		c_lefttime = put_left_time;
		c_priority = put_priority;

		if (buffer_t != NULL) {
			output[output_index++] = buffer_t;
			buffer_t = NULL;
			s_put_flag = true;
			printf("|-----------------------------S_PUT--------------------------|\n");
			printf("|output_index = %d ; buffer_t = %c      \n", output_index, buffer_t);
			PrintInfo();
			printf("|-----------------------------S_PUT--------------------------|\n");
			printf("\n\n\n\n");
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		lock.unlock();
	}
}

void s_display()
{
	while (s_flag) {
		lock.lock();

		if (s_put_flag) {
			printf("|----------------------------Display-------------------------|\n");
			printf("|输出内容为: %s \n", output);
			printf("|----------------------------Display-------------------------|\n");
			printf("\n\n\n\n");
			s_put_flag = false;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		lock.unlock();
	}
}

int a_exe(const char* args)
{
	s_flag = true;

	ReadFile("MYFILE.txt");

	using std::thread;
	thread p_get(s_get), p_copy(s_copy), p_put(s_put), p_display(s_display);

	p_get.detach();
	p_copy.detach();
	p_put.detach();
	p_display.detach();


	return 0;
}