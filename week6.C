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
#define N_TASK 4

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

OS_EVENT *sem_decision[4]; // decision task용 semaphore
OS_EVENT *sem_random[4]; // random task용 semaphore

OS_FLAG_GRP *r_grp; // event flag
OS_FLAG_GRP *s_grp;

INT8U err; // event flag 생성 위한 err 변수.

int send_array[4]; // 공유 변수
char receive_array[4]; // 공유 변수 -> **여기있는 거 건드리면 dosbox 꺼짐**

// ***********여기서부터 실습에 사용한 변수 (6주차 실습)**************

OS_EVENT *mbox_to_master[3]; // mailbox
OS_FLAG_GRP *d_grp; // event flag

int majority_cnt = -1; // 다수결을 정하는 공유 변수, 0 : O, 1 : X

INT8U select = 2; // 사용자가 커맨드에 1이나 2를 입력, 1 : semaphore, 2 : event flag 

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

// 임베디드소프트웨어 6주차 실습
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
	
	for (i = 0; i < 3; i++)
	{
		mbox_to_master[i] = OSMboxCreate((void *)0); // mailbox 생성
	}
	d_grp = OSFlagCreate(0x00, &err); // event flag 생성

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
	int get_number[4];

	INT8U i, j;

	INT8U min;		// 최솟값을 담을 변수
	INT8U min_task;
	int task_number = (int)(*(char*)pdata - 48);//각 task의 index이다. pdata는 char타입이기 때문에 ascii 기준 -48을 하면 int형으로 바뀐다.

	char push_letter;//W OR L을 저장 Decision task에서 생성한다.
	char get_letter;//W or L을 저장 decision task에서 만든 문자를 Random task1~4중 하나가 받는다.


	int fgnd_color, bgnd_color;// 배경색

	char s[10];//

	int rnd_n; // O, X 정하는 랜덤 변수 (0 : O, 1 : X)
	char OX_letter; // 각 task의 O / X 저장하는 변수
	char OX_arr[3]; // master task에서 다른 voting task의 O, X 저장하는 배열
	int O_cnt; // O의 개수, 2 이상이면 O가 다수결

	//task_number, pdata가 0이면 master task, 1~2이면 voting task, 3이면 decision task
	if (*(char*)pdata == '3') { // decision task
		for (;;) {
			
			// 다수결 정할 때까지 공유변수 기다림 (majority_cnt)
			OSFlagPend(d_grp, 0x0F, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

			if (majority_cnt == 0) // O : 파란색
			{
				bgnd_color = DISP_BGND_BLUE;
				fgnd_color = DISP_FGND_BLUE;
			}
			else // X : 빨간색
			{
				bgnd_color = DISP_BGND_RED;
				fgnd_color = DISP_FGND_RED;
			}
			
			for (j = 5; j < 24; j++) { // 배경 칠하기
				for (i = 0; i < 80; i++) {
					PC_DispChar(i, j, ' ', fgnd_color + bgnd_color);
				}
			}

			OSTimeDlyHMSM(0, 0, 5, 0);
		}
	}
	else if (*(char*)pdata == '0') // master task
	{
		for (;;)
		{
			rnd_n = random(2); // 0, 1 중 랜덤하게 선택
			if (rnd_n == 0) OX_letter = 'O';
			else OX_letter = 'X';

			// 출력
			PC_DispStr(8 + 18 * task_number, 4, "[", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispStr(10 + 18 * task_number, 4, "]", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispChar(9 + 18 * task_number, 4, OX_letter, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);

			OX_arr[0] = OX_letter; // 0번 task(자기 자신)
			for (i = 1; i < 3; i++)
			{ // voting task의 O/X 기다림 (1번, 2번)
				OX_arr[i] = *(char*)OSMboxPend(mbox_to_master[i], 0, &err); 
			}

			O_cnt = 0; // O을 세는 카운트
			for (i = 0; i < 3; i++)
			{
				if (OX_arr[i] == 'O') O_cnt++;
				OSFlagPost(d_grp, 0x01 << i, OS_FLAG_SET, &err);
			}

			// 다수결 선택 (공유변수 majority_cnt 이용)
			if (O_cnt >= 2) majority_cnt = 0;
			else majority_cnt = 1;
			
			// decision task에 flag 전송 (다수결 선택 완료)
			OSFlagPost(d_grp, 0x01 << 3, OS_FLAG_SET, &err);

			OSTimeDlyHMSM(0, 0, 5, 0);
		}
	}
	else { // voting task

		for (;;) {
			rnd_n = random(2); // 0, 1 중 랜덤하게 선택
			if (rnd_n == 0) OX_letter = 'O';
			else OX_letter = 'X';

			// 출력
			PC_DispStr(8 + 18 * task_number, 4, "[", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispStr(10 + 18 * task_number, 4, "]", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispChar(9 + 18 * task_number, 4, OX_letter, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			
			// master task로 O/X 전송
			OSMboxPost(mbox_to_master[task_number], (void*)&OX_letter);			
			
			OSTimeDlyHMSM(0, 0, 5, 0);
		}
	}
}