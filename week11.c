/* GPIO로 LED 켜고 끄기 1번 예
 LED 8개가 연결되어 있는 포트 : Port A(PA)
 - 비트7 : LED7(LD1), 비트6 : LED6(LD2)
 - ...
 - 비트1 : LED1(LD7), 비트0 : LED0(LD8) */

 // 임베디드소프트웨어 11주차 실습
 // 12161569 컴퓨터공학과 박도윤

 #include <avr/io.h>  // ATmage128 레지스터 정의
 #include <util/delay.h> // delay
 #include <avr/interrupt.h>
 
 #define F_CPU 16000000UL
 
 #define DO 17
 #define RE 43
 #define MI 66
 #define FA 77
 #define SOL 96
 #define LA 114
 #define TI 129
 #define UDO 136

 #define ON 1
 #define OFF 0

 // Buzzer 제어 프로그램 1
 /*
 const int scale[8] = {DO, RE, MI, FA, SOL, LA, TI, UDO};
 int state = ON; // 주파수 발생시키기 위해 소리를 끄고 켤 변수
 int index = 0; // 몇번째 음을 발생시킬 지 결정할 변수
 int flag = 0; // 스위치가 중복으로 눌리는 것을 보정하기 위한 변수

 ISR(TIMER0_OVF_vect){ // overflow
 	if(state == ON)
	{
		PORTB = 0x00; // state on -> buzzer 출력값 1 설정
		state = OFF; // change state
	}
	else
	{
		PORTB = 0x10; // state off -> buzzer 출력값 0 설정
		state = ON; // change state
	}
	TCNT0 = scale[index]; // 현재 index에 해당하는 소리 출력
	flag = 1; // 중복 눌림을 방지하기 위해 한단계 음 상승 후 flag 전환
 }

 ISR(INT5_vect){ // interrupt 5
 	if(flag == 1) // flag 1일 경우에만 index 1 증가
	{	
		index = (index + 1) % 8; flag = 0; // 증가 후 flag = 0으로 설정
		_delay_ms(3000); // delay 3 sec
	}
 }

 int main()
 {
	// 1. 버저 출력으로 설정
	DDRB = 0x10;
	// 2. 스위치 입력 설정 -> SW2 enable
	DDRE = 0xDF; // 1101 1111
	// 3. interrupt 5 enable (0010 0000)
	EIMSK = 0x20; // 0010 0000
	// 4. 하강에지에서 인터럽트 발생
	EICRB = 0x08; // 0000 1000
	// 5. global interrupt 활성화
	sei();
	// 6. 32분주
	TCCR0 = 0x03; // 0000 0011
	// 7. overflow 되도록 설정
	TIMSK = 0x01;
 	
 	for(;;)
	{
		
	}

	return 0;
 }
*/

// Buzzer 제어 프로그램 2

const unsigned char melody[24] = {SOL, SOL, LA, LA, SOL, SOL, MI,
	SOL, SOL, MI, MI, RE, SOL, SOL, LA, LA, SOL, SOL, MI, SOL, MI, RE, MI, DO};
int index = 0;
int state = ON;
int cnt = 0;
// 각 멜로디의 음마다 필요한 interval 배열
const int interval[24] = {2000, 2000, 2000, 2000, 2000, 2000, 5000, 2000, 2000, 2000, 2000, 5000,
	2000, 2000, 2000, 2000, 2000, 2000, 5000, 2000, 2000, 2000, 2000, 5000};

// 각 멜로디의 주파수를 2로 나눈 배열 -> 음마다 같은 시간 동안 소리 내기 위해서
const int change[24] = {784, 784, 880, 880, 784, 784, 659, 784, 784, 659, 659, 587,
	784, 784, 880, 880, 784, 784, 659, 784, 659, 587, 659, 523};

ISR(TIMER0_OVF_vect){ // overflow
 	if(state == ON)
	{
		PORTB = 0x00; // state on -> buzzer 출력값 1 설정
		state = OFF; // change state
	}
	else
	{
		PORTB = 0x10; // state off -> buzzer 출력값 0 설정
		state = ON; // change state
	}
	TCNT0 = melody[index]; // 현재 index에 해당하는 소리 출력
	cnt++;
	if(cnt == change[index]) // count 다 채웠다면 (다음 멜로디로 넘어감)
	{	
		cnt = 0; // 초기화
		_delay_ms(interval[index]); // delay
		index = (index + 1) % 24;
	}
 }

 int main()
 {	
	DDRB = 0x10; // 버저 출력 설정
	TCCR0 = 0x03; // 32분주, 0000 0011
	TIMSK = 0x01; // overflow
	sei(); // 전역 인터럽트 enable

	while(1)
	{
		
	}
 	return 0;
 }

