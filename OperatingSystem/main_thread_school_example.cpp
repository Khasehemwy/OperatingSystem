#pragma warning(disable : 4996)

//#include "afxmt.h"
//#include <windows.h>
#include "afxwin.h"
#include "stdio.h"
//#include <conio.h>

CRITICAL_SECTION g_cs;  // �ٽ����ṹ����
char buffer_s = NULL;   // ������Դ
char buffer_t = NULL;   // ������Դ
int input_index = 0;    //input���鵱ǰָ��λ��
int output_index = 0;   //input���鵱ǰָ��λ��
char input[1000];
char output[1000];
bool s_flag;      //�ṩѭ��
bool s_put_flag;  //ͬ������
const char* c_process;
int c_runtime, c_lefttime, c_priority;


void readfile() {
    char ch;
    FILE* stream;
    stream = fopen("MYFILE.TXT", "rb");
    while ((ch = fgetc(stream)) != EOF) {
        input[input_index] = ch;
        input_index++;
        //printf("%c\n",ch);
    }
    printf("%s\n", input);
    input_index = 0;

    fclose(stream);
}

UINT display(LPVOID pParam) {
    while (s_flag) {
        EnterCriticalSection(&g_cs);  //�����ٽ���
        if (s_put_flag) {
            printf("|----------------------------Display-------------------------| \n\n");
            //printf("|��������Ϊ:%s\n", input);
            printf("|�������Ϊ:%s \n\n", output);
            printf("|----------------------------Display-------------------------|\n");
            printf("\n\n\n\n");
            s_put_flag = false;
            Sleep(3000);
        }
        LeaveCriticalSection(&g_cs);  //�뿪�ٽ���
    }
    return 0;
}

UINT s_get(LPVOID pParam) {
    int get_run_time = 0;
    int get_left_time = 1000;
    int get_priority = 1;
    while (s_flag) {
        EnterCriticalSection(&g_cs);  // �����ٽ���
        get_run_time = get_run_time + 10;
        get_left_time = get_left_time - 10;
        c_process = "s_get";
        c_runtime = get_run_time;
        c_lefttime = get_left_time;
        c_priority = get_priority;

        if (buffer_s == NULL) {
            buffer_s = input[input_index];
            input_index++;
            printf("|-----------------------------S_GET--------------------------|\n");
            printf("|input_index = %d and buffer_s = %c        \n\n", input_index, buffer_s);
            printf("|  ��ǰ���� |   ������ʱ�� |   ����Ҫʣ��ʱ�� |  �������ȼ�  |\n");
            printf("|   %s          %d             %d                  %d   \n\n", c_process, c_runtime, c_lefttime, c_priority);
            printf("|-----------------------------S_GET--------------------------|\n");
            printf("\n\n\n\n");
            Sleep(3000);
        }
        LeaveCriticalSection(&g_cs);  //�뿪�ٽ���
    }
    return 0;
}

UINT s_copy(LPVOID pParam) {
    int copy_run_time = 0;
    int copy_left_time = 1000;
    int copy_priority = 2;
    while (s_flag) {
        EnterCriticalSection(&g_cs);  //�����ٽ���
        copy_run_time = copy_run_time + 10;
        copy_left_time = copy_left_time - 10;
        c_process = "s_copy";
        c_runtime = copy_run_time;
        c_lefttime = copy_left_time;
        c_priority = copy_priority;
        if (buffer_s != NULL) {
            if (buffer_t == NULL) {
                buffer_t = buffer_s;
                buffer_s = NULL;
                printf("|-----------------------------S_COPY-------------------------|\n");
                printf("|             copying from buffer_s to buffer_t......... \n\n");
                printf("|  ��ǰ���� |   ������ʱ�� |   ����Ҫʣ��ʱ�� |  �������ȼ�  |\n");
                printf("|   %s          %d             %d                  %d   \n\n", c_process, c_runtime, c_lefttime, c_priority);
                printf("|-----------------------------S_COPY-------------------------|\n");
                printf("\n\n\n\n");
                Sleep(3000);
            }
        }
        LeaveCriticalSection(&g_cs);  //�뿪�ٽ���
    }
    return 0;
}

UINT s_put(LPVOID pParam) {
    int put_run_time = 0;
    int put_left_time = 1000;
    int put_priority = 3;

    while (s_flag) {
        EnterCriticalSection(&g_cs);  //�����ٽ���
        put_run_time = put_run_time + 10;
        put_left_time = put_left_time - 10;
        c_process = "s_put";
        c_runtime = put_run_time;
        c_lefttime = put_left_time;
        c_priority = put_priority;
        if (buffer_t != NULL) {
            output[output_index] = buffer_t;
            output_index++;
            printf("|-----------------------------S_PUT--------------------------|\n");
            printf("|output_index = %d and buffer_t = %c      \n\n", output_index, buffer_t);
            printf("|  ��ǰ���� |   ������ʱ�� |   ����Ҫʣ��ʱ�� |  �������ȼ�  |\n");
            printf("|   %s          %d             %d                  %d   \n\n", c_process, c_runtime, c_lefttime, c_priority);
            printf("|-----------------------------S_PUT--------------------------|\n");
            printf("\n\n\n\n");
            buffer_t = NULL;
            s_put_flag = true;
            Sleep(3000);
        }
        LeaveCriticalSection(&g_cs);  //�뿪�ٽ���
    }
    //AfxEndThread(0, TRUE);
    return 0;
}

void main() {
    s_flag = true;
    s_put_flag = false;
    readfile();
    InitializeCriticalSection(&g_cs);  //��ʼ���ٽ���
    CWinThread* p_get, * p_copy, * p_put, * p_display;

    p_get = AfxBeginThread(s_get, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    p_get->ResumeThread();

    p_copy = AfxBeginThread(s_copy, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    p_copy->ResumeThread();

    p_put = AfxBeginThread(s_put, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    p_put->ResumeThread();

    p_display = AfxBeginThread(display, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
    p_display->ResumeThread();

    system("pause");  //�ȴ���������̽�����
}