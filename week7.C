/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #2
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define          TASK_STK_SIZE     512                /* Size of each task's stacks (# of WORDs)       */
#define N_TASK 5

/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

OS_STK TaskStartStk[TASK_STK_SIZE];
OS_STK TaskStk[N_TASK][TASK_STK_SIZE];
char TaskData[N_TASK];

OS_EVENT randomsem;
OS_EVENT *sem;

INT8U err; // event flag 생성 위한 err 변수.

// ***********여기서부터 실습에 사용한 변수 (7주차 실습)**************

OS_EVENT *queue_to_comp; // message queue
void *comp_queue[3];

OS_FLAG_GRP *d_grp; // display task로 넘겨받는 event flag
OS_FLAG_GRP *notify_grp; // display task에서 다른 task에게 알리는 event flag

char disp_sign; // 공유 변수, display task에 어떤 색 칠해야 하는지 알려주는 변수

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void Task(void * data);
static  void  TaskStart(void *data);                  /* Function prototypes of tasks                  */
static  void  TaskStartCreateTasks(void);
static  void  TaskStartDispInit(void);
static  void  TaskStartDisp(void);


/*$PAGE*/
/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

// 임베디드소프트웨어 7주차 실습
// 12161569 컴퓨터공학과 박도윤

void main(void)
{
	OS_STK *ptos;
	OS_STK *pbos;
	INT32U  size;

	PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);                        /* Clear the screen                         */

	OSInit();                                              /* Initialize uC/OS-II                      */

	PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */
	PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */

	OSTaskCreate(TaskStart, (void*)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);

	OSStart();                                             /* Start multitasking                       */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               STARTUP TASK
*********************************************************************************************************
*/

static void  TaskStart(void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
	OS_CPU_SR  cpu_sr;
#endif
	INT16S     key;
	INT8U i;

	pdata = pdata;                                         /* Prevent compiler warning                 */

	TaskStartDispInit();                                   /* Setup the display                        */

	OS_ENTER_CRITICAL();                                   /* Install uC/OS-II's clock tick ISR        */
	PC_VectSet(0x08, OSTickISR);
	PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
	OS_EXIT_CRITICAL();

	OSStatInit();                                          /* Initialize uC/OS-II's statistics         */

	sem = OSSemCreate(0);

	// message queue 생성
	queue_to_comp = OSQCreate(&comp_queue[0], 3);

	// event flag 생성
	d_grp = OSFlagCreate(0x00, &err);
	notify_grp = OSFlagCreate(0x00, &err);

	TaskStartCreateTasks();                                /* Create all other tasks                   */

	for (;;) {
		TaskStartDisp();                                   /* Update the display                       */

		if (PC_GetKey(&key)) {                             /* See if key has been pressed              */
			if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
				PC_DOSReturn();                            /* Yes, return to DOS                       */
			}
		}

		OSCtxSwCtr = 0;                                    /* Clear context switch counter             */
		OSTimeDly(OS_TICKS_PER_SEC);                       /* Wait one second                          */
	}
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                        INITIALIZE THE DISPLAY
*********************************************************************************************************
*/

static  void  TaskStartDispInit(void)
{
	/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
	/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
	PC_DispStr(0, 0, "                         uC/OS-II, The Real-Time Kernel                         ", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
	PC_DispStr(0, 1, "                                Jean J. Labrosse                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 2, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 3, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 5, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 6, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 7, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 8, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 9, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 10, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 13, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 14, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 15, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 18, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 19, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 20, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 21, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 22, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 23, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 24, "                            <-PRESS 'ESC' TO QUIT->                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY + DISP_BLINK);
	/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
	/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                           UPDATE THE DISPLAY
*********************************************************************************************************
*/

static  void  TaskStartDisp(void)
{
	char   s[80];


	sprintf(s, "%5d", OSTaskCtr);                                  /* Display #tasks running               */
	PC_DispStr(18, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	sprintf(s, "%3d", OSCPUUsage);                                 /* Display CPU usage in %               */
	PC_DispStr(36, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	sprintf(s, "%5d", OSCtxSwCtr);                                 /* Display #context switches per second */
	PC_DispStr(18, 23, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	sprintf(s, "V%4.2f", (float)OSVersion() * 0.01);               /* Display uC/OS-II's version number    */
	PC_DispStr(75, 24, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	switch (_8087) {                                               /* Display whether FPU present          */
	case 0:
		PC_DispStr(71, 22, " NO  FPU ", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 1:
		PC_DispStr(71, 22, " 8087 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 2:
		PC_DispStr(71, 22, "80287 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 3:
		PC_DispStr(71, 22, "80387 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             CREATE TASKS
*********************************************************************************************************
*/

static  void  TaskStartCreateTasks(void)
{
	INT8U i;

	for (i = 0; i < N_TASK; i++) {
		TaskData[i] = '0' + i;
		OSTaskCreate(Task, (void*)&TaskData[i], &TaskStk[i][TASK_STK_SIZE - 1], i + 1);
	}
}

void Task(void * pdata) {
	INT8U err;

	INT8U push_number;

	INT8U i, j;

	int task_number = (int)(*(char*)pdata - 48);//각 task의 index이다. pdata는 char타입이기 때문에 ascii 기준 -48을 하면 int형으로 바뀐다.

	char push_letter;//W OR L을 저장 Decision task에서 생성한다.
	char get_letter;//W or L을 저장 decision task에서 만든 문자를 Random task1~4중 하나가 받는다.


	int fgnd_color, bgnd_color;// 배경색

	char s[10]; // 화면에 랜덤 숫자 출력하기 위한 변수

	int rnd_n; // 랜덤으로 부여받는 번호
	int get_number[3]; // number generation task로부터 전달받는 숫자 배열

	INT8U max;		// 최댓값을 담을 변수
	INT8U max_task; // 최댓값 가지는 task 번호 (0, 1, 2)
	

	//task_number, pdata가 0 -> display task, 1 -> comparison task, 2, 3, 4 -> number generation task
	if (*(char*)pdata == '0') { // display task
		for (;;) {
			// display 정보 넘겨받기 전까지 대기
			OSFlagPend(d_grp, 0x0F, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

			// 배경색 설정 (공유 변수 확인)
			if (disp_sign == 'R')
			{
				bgnd_color = DISP_BGND_RED;
				fgnd_color = DISP_FGND_RED;
			}
			else if (disp_sign == 'G')
			{
				bgnd_color = DISP_BGND_GREEN;
				fgnd_color = DISP_FGND_GREEN;
			}
			else
			{
				bgnd_color = DISP_BGND_BLUE;
				fgnd_color = DISP_FGND_BLUE;
			}

			for (j = 5; j < 24; j++) { // 배경 칠하기
				for (i = 0; i < 80; i++) {
					PC_DispChar(i, j, ' ', fgnd_color + bgnd_color);
				}
			}

			// 다른 task에게 flag 전송
			for (i = 0; i < 4; i++)
			{
				OSFlagPost(notify_grp, 0x01 << i, OS_FLAG_SET, &err);
			}
			
			OSTimeDlyHMSM(0, 0, 3, 0);
		}
	}
	else if (*(char*)pdata == '1') // comparison task
	{
		for (;;)
		{
			for (i = 0; i < 3; i++)
			{
				get_number[i] = *(int*)OSQPend(queue_to_comp, 0, &err); // 랜덤 숫자 넘겨받기
			}

			// 최댓값 구하기
			max = get_number[0];
			max_task = 0;
			for (i = 1; i < 3; i++)
			{
				if (max < get_number[i])
				{
					max = get_number[i];
					max_task = i;
				}
			}
			if (max_task == 0) disp_sign = 'R';
			else if (max_task == 1) disp_sign = 'B';
			else disp_sign = 'G';

			// 중복인지 확인
			if (get_number[0] == get_number[1] || get_number[1] == get_number[2] ||
				get_number[0] == get_number[2])
			{
				disp_sign = 'R';
			}
						
			// display task에 flag 전송
			OSFlagPost(d_grp, 0x01, OS_FLAG_SET, &err);

			// display task의 flag 대기
			OSFlagPend(notify_grp, 0x01 << (task_number - 1), OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

			OSTimeDlyHMSM(0, 0, 3, 0);
		}
	}
	else { // number generation task

		for (;;) {
			rnd_n = random(64); // 랜덤 숫자 생성
			sprintf(s, "%2d", rnd_n); // 랜덤 숫자 출력위해 string으로 형변환

			// 출력
			PC_DispStr(8 + 18 * (task_number - 2), 4, "[", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispStr(11 + 18 * (task_number - 2), 4, "]", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispStr(9 + 18 * (task_number - 2), 4, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);

			OSQPost(queue_to_comp, (void *)&rnd_n); // comparision task에 랜덤 숫자 전송

			// flag 전송 (0010, 0100, 1000)
			OSFlagPost(d_grp, 0x01 << (task_number - 1), OS_FLAG_SET, &err);

			// display task의 flag 대기
			OSFlagPend(notify_grp, 0x01 << (task_number - 1), OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

			OSTimeDlyHMSM(0, 0, 3, 0);
		}
	}
}