/* GPIO로 LED 켜고 끄기 1번 예
 LED 8개가 연결되어 있는 포트 : Port A(PA)
 - 비트7 : LED7(LD1), 비트6 : LED6(LD2)
 - ...
 - 비트1 : LED1(LD7), 비트0 : LED0(LD8) */

 // 임베디드소프트웨어 10주차 실습
 // 12161569 컴퓨터공학과 박도윤

 #include <avr/io.h>  // ATmage128 레지스터 정의
 #include <util/delay.h> // delay
 #include <stdlib.h> // 랜덤 함수 rand()
 #include <avr/interrupt.h>
 
 #define F_CPU 16000000UL
 #define START 1
 #define STOP 0

typedef unsigned char uc;
const uc digit[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F};
const uc find_sel[4] = {0x08, 0x04, 0x02, 0x01};
const uc dot = 0x80;

// FND 실습 과제1 - 초시계 만들기
/*
int printNum(unsigned char arr[4])
{
	// your code
	for(int i = 0; i < 4; i++)
	{
		PORTC = arr[i];
		PORTG = find_sel[i];
		_delay_us(2500); // 100분의 1초 (2500 * 4)
	}
	return 1;
}

int main()
{
	int i, j, k, l;
	unsigned char num[4];

	// your code
	DDRC = 0xFF; // C PORT 출력 포트 설정
	DDRG = 0x0F; // G PORT 선택 출력 설정


	for(i = 0;; i++)
	{
		if(i == 10) i = 0;
		num[0] = digit[i];
		for(j = 0; j < 10; j++)
		{
			num[1] = digit[j] + dot;
			for(k = 0; k < 10; k++)
			{
				num[2] = digit[k];
				for(l = 0; l < 10; l++)
				{
					num[3] = digit[l];
					printNum(num);
				}
			}
		}
	}
	return 0;
}
*/

// LED + FND 실습 과제2 - LED 위치에 매핑되는 숫자 출력하기 (0~7)
/*
int main()
{
	DDRA = 0xFF; // LED 출력 설정
	DDRC = 0xFF; // C PORT 출력 포트 설정
	DDRG = 0x0F; // G PORT 선택 출력 설정
	
	int rand_num;

	for(;;)
	{
		rand_num = rand() % 8; // 랜덤 수 생성

		PORTA = 1 << rand_num;
		for(int j = 0; j < 40; j++)
		{
			for(int i = 0; i < 4; i++)
			{
				PORTC = digit[rand_num];
				PORTG = find_sel[i];
				_delay_ms(100);
			}
		}
				
	}


	return 0;
}
*/

// Switch + Stop Watch 실습 과제3
// SW1 한번 누르면 Start, 다시 누르면 Stop
// SW2 누르면 초기화

int count = 0;
unsigned char num[4];
int signal;

ISR(INT4_vect) // start and stop
{
	if(signal == START) signal = STOP;
	else signal = START;
	_delay_ms(10);
}

ISR(INT5_vect) // init
{
	if(signal == STOP)
	{
		// your code
		count = 0;
		//signal = START;

		_delay_ms(10);
	}
}

void display_fnd(int c)
{	
	// your code
	// 7 segment 출력
	num[0] = (c / 1000) % 10;
	num[1] = (c / 100) % 10;
	num[2] = (c / 10) % 10;
	num[3] = c % 10;

	for(int i = 0; i < 4; i++)
	{
		if(i == 1)
		{	
			PORTC = digit[num[i]] + dot;
		}
		else PORTC = digit[num[i]];
		PORTG = find_sel[i];
		_delay_us(2500); // 100분의 1초 (2500 * 4)
	}
}

int main()
{
	// your code, port setting
	// port setting -> FND & INT4, INT5 enable
	DDRC = 0xFF; // FND
	DDRG = 0x0F; 
	DDRE = 0xCF; // 0b11001111
	EICRB = 0x0A; // 0b00001010, INT4, INT5 falling edge
	EIMSK = 0x30; // 0b00110000, INT4, INT5 enable
	sei(); // SREG bit7, interrupt enable

	signal = STOP; // 처음에 signal 초기화

	for(;;)
	{	
		display_fnd(count);
		if(signal == START) count = (count + 1) % 10000;
	}
		
	return 0;
}




