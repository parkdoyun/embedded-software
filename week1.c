#include <stdio.h>

// 임베디드소프트웨어 1주차 실습
// 12161569 컴퓨터공학과 박도윤

int LED_function(int input); // exercise 1

int main()
{
	// exercise 1
	
	unsigned char LEDs;

	int num;
	scanf("%d", &num); // shift할 숫자 입력 (0~7)
	LEDs = LED_function(num); 
	printf("return_value : %d\n", LEDs);
	//printf("LEDs : (10진수)%d, (16진수)%X\n", LEDs, LEDs); // 출력 (10진수, 16진수)
	
	
	// exercise 2
	/*
	for (int i = 0; i < 4; i++)
	{
		int value = 1;
		while (value != 0x80) // 1000 0000일때까지
		{
			printf("%d, ", value);
			value = (value << 1); // 계속 왼쪽으로 shift
		}
		printf("%d, ", value); // 1000 0000 출력
		value = (value >> 1); // 역으로 오른쪽으로 1 shift
		printf("%d\n", value);			
	}
	*/

	// exercise 3
	/*
	unsigned char LEDs;
	scanf("%uc", &LEDs); // 입력

	// 1과의 AND로 1 검출
	int cmp_bit = 1; // AND로 비교할 비트
	int one_num = 0; // 입력받은 LEDs의 1의 개수

	for (int i = 0; i < 8; i++) // 1의 개수 검사
	{
		int res = LEDs & cmp_bit; // AND 비교
		if (res != 0) one_num++; // 결과가 0이 아니다 -> 비교한 비트가 1이다
		cmp_bit = (cmp_bit << 1);
	}
	printf("1의 개수? %d\n", one_num);

	int shift_num = 8 - one_num; // shift할 개수
	unsigned char value = 0xFF;
	value = (value << shift_num); // 0xFF를 왼쪽으로 shift
	printf("Shift 시의 값은? %d\n", value);
	//printf("%d, %X\n", value, value); // 10진수, 16진수로 출력
	*/

	// exercise 4
	/*
	// bit manipulation
	// 0~7 비트 전부 clear
	unsigned char c = 0x7C;
	unsigned char cmp_bit = 0x00;
	c = c & cmp_bit;
	printf("all bit clear : %X\n", c);

	// 0~7 비트 전부 set
	c = 0x5B;
	cmp_bit = 0xFF;
	c = c | cmp_bit;
	printf("all bit set : %X\n", c);
	*/

	return 0;
}

int LED_function(int input) // exercise 1
{
	int value = (1 << input);
	return value;
}