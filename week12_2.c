#include "includes.h"

#define F_CPU	16000000UL	// CPU frequency = 16 Mhz
#include <avr/io.h>	
#include <util/delay.h>

#define  TASK_STK_SIZE  OS_TASK_DEF_STK_SIZE            

OS_STK          LedTaskStk[TASK_STK_SIZE];


void  LedTask(void *data);

// 임베디드소프트웨어 12주차 실습
// 12161569 컴퓨터공학과 박도윤
// 실습과제 2 - porting

int main (void)
{
  OSInit();

  OS_ENTER_CRITICAL();
  TCCR0=0x07;  
  TIMSK=_BV(TOIE0);                  
  TCNT0=256-(CPU_CLOCK_HZ/OS_TICKS_PER_SEC/ 1024);   
  OS_EXIT_CRITICAL();
  
  DDRA = 0xFF;
  OSTaskCreate(LedTask, (void *)0, (void *)&LedTaskStk[TASK_STK_SIZE - 1], 0);

  OSStart();                         
  
  return 0;
}


void LedTask (void *data)
{
  data = data;    
  // LED Task
  while (1) // true
  {
	  PORTA = 0xFF; // on
	  _delay_ms(500);
	  PORTA = 0x00; // off
	  _delay_ms(500);
  }
  
}
