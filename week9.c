/* GPIO로 LED 켜고 끄기 1번 예
 LED 8개가 연결되어 있는 포트 : Port A(PA)
 - 비트7 : LED7(LD1), 비트6 : LED6(LD2)
 - ...
 - 비트1 : LED1(LD7), 비트0 : LED0(LD8) */

 // 임베디드소프트웨어 9주차 실습
 // 12161569 컴퓨터공학과 박도윤

 #include <avr/io.h>  // ATmage128 레지스터 정의
 #include <util/delay.h> // delay
 #define F_CPU 16000000UL

// LED 실습 과제
// 좌우로 LED 이동
/*
 void main(){
 	unsigned char value = 1;
 	DDRA= 0xff; // 포트 A를 출력 포트로 사용
	int direction = 0; // 0 : left, 1 : right
	for(;;)
	{
		PORTA = value; // print
		_delay_ms(1000); // delay
		if(direction == 0) // left
		{
			value <<= 1;
			if(value == 128) direction = 1; // direction change
		}
		else // direction  == 1, right
		{
			value >>= 1;
			if(value == 1) direction = 0; // direction change
		}

	} 

 }
 */
 
 // LED 자유 과제

void main(){
 	// 악보가 되는 배열 (학교 종이 땡땡땡)
	// LED를 왼쪽부터 도레미파솔라시도로 설정
	// 솔솔라라 솔솔미 솔솔미미레레
	// 솔솔라라 솔솔미 솔미레미도도
	unsigned char music[28] = {0x08, 0x08, 0x04, 0x04, 0x08, 0x08, 0x20, 0x20, 0x08, 0x08, 0x20, 0x20, 0x40, 0x40,
	0x08, 0x08, 0x04, 0x04, 0x08, 0x08, 0x20, 0x20, 0x08, 0x20, 0x40, 0x20, 0x80, 0x80};
	
	DDRA = 0xFF;
	int idx = 0; // 악보의 인덱스
	for(;;)
	{
		PORTA = music[idx];
		_delay_ms(5000); // delay
		idx = (idx + 1) % 28; // index
	}
}

