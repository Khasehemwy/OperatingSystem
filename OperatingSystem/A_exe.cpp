#include"A_exe.h"

namespace A_exe {
	char input[1000];
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
};

void A_exe::ReadFile(const char* filePath)
{
	char ch;
	FILE* stream;
	stream = fopen(filePath, "rb");
	while ((ch = fgetc(stream)) != EOF) {
		input[input_index] = ch;
		input_index++;
		input_n++;
	}
	input_index = 0;

	fclose(stream);
}

void A_exe::PrintInfo()
{
	printf("| current process | run time | left time | priority  |\n");
	printf("|   %s          %d             %d                %d   \n", c_thread, c_runtime, c_lefttime, c_priority);
}

void A_exe::s_get()//将input放入buffer_s
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

		if (buffer_s == NULL && input_index < input_n) {
			buffer_s = input[input_index++];
			printf("|-----------------------------S_GET--------------------------|\n");
			printf("|input_index = %d ; buffer_s = %c        \n", input_index, buffer_s);
			PrintInfo();
			printf("|-----------------------------S_GET--------------------------|\n");
			printf("\n\n\n\n");
			std::this_thread::sleep_for(std::chrono::seconds(4));
		}

		lock.unlock();
	}
}

void A_exe::s_copy()//将buffer_s放入buffer_t
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
				std::this_thread::sleep_for(std::chrono::seconds(4));
			}
		}

		lock.unlock();
	}
}

void A_exe::s_put()//将buffer_t放入output
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
			printf("|-----------------------------S_PUT--------------------------|\n");
			printf("|output_index = %d ; buffer_t = %c      \n", output_index, buffer_t);
			PrintInfo();
			printf("|-----------------------------S_PUT--------------------------|\n");
			printf("\n\n\n\n");
			buffer_t = NULL;
			s_put_flag = true;

			std::this_thread::sleep_for(std::chrono::seconds(4));
		}

		lock.unlock();
	}
}

void A_exe::s_display()
{
	while (s_flag) {
		lock.lock();

		if (s_put_flag) {
			printf("|----------------------------Display-------------------------|\n");
			printf("|output: %s \n", output);
			printf("|----------------------------Display-------------------------|\n");
			printf("\n\n\n\n");
			s_put_flag = false;
			std::this_thread::sleep_for(std::chrono::seconds(4));
		}

		lock.unlock();
	}
}

int A_exe::main(const char* args)
{
	s_flag = true;

	ReadFile("Resources/MYFILE.txt");

	using std::thread;
	thread p_get(s_get), p_copy(s_copy), p_put(s_put), p_display(s_display);

	p_get.detach();
	p_copy.detach();
	p_put.detach();
	p_display.detach();


	return 0;
}
