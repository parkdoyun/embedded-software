#include <stdio.h>

// �Ӻ�������Ʈ���� 1���� �ǽ�
// 12161569 ��ǻ�Ͱ��а� �ڵ���

int LED_function(int input); // exercise 1

int main()
{
	// exercise 1
	
	unsigned char LEDs;

	int num;
	scanf("%d", &num); // shift�� ���� �Է� (0~7)
	LEDs = LED_function(num); 
	printf("return_value : %d\n", LEDs);
	//printf("LEDs : (10����)%d, (16����)%X\n", LEDs, LEDs); // ��� (10����, 16����)
	
	
	// exercise 2
	/*
	for (int i = 0; i < 4; i++)
	{
		int value = 1;
		while (value != 0x80) // 1000 0000�϶�����
		{
			printf("%d, ", value);
			value = (value << 1); // ��� �������� shift
		}
		printf("%d, ", value); // 1000 0000 ���
		value = (value >> 1); // ������ ���������� 1 shift
		printf("%d\n", value);			
	}
	*/

	// exercise 3
	/*
	unsigned char LEDs;
	scanf("%uc", &LEDs); // �Է�

	// 1���� AND�� 1 ����
	int cmp_bit = 1; // AND�� ���� ��Ʈ
	int one_num = 0; // �Է¹��� LEDs�� 1�� ����

	for (int i = 0; i < 8; i++) // 1�� ���� �˻�
	{
		int res = LEDs & cmp_bit; // AND ��
		if (res != 0) one_num++; // ����� 0�� �ƴϴ� -> ���� ��Ʈ�� 1�̴�
		cmp_bit = (cmp_bit << 1);
	}
	printf("1�� ����? %d\n", one_num);

	int shift_num = 8 - one_num; // shift�� ����
	unsigned char value = 0xFF;
	value = (value << shift_num); // 0xFF�� �������� shift
	printf("Shift ���� ����? %d\n", value);
	//printf("%d, %X\n", value, value); // 10����, 16������ ���
	*/

	// exercise 4
	/*
	// bit manipulation
	// 0~7 ��Ʈ ���� clear
	unsigned char c = 0x7C;
	unsigned char cmp_bit = 0x00;
	c = c & cmp_bit;
	printf("all bit clear : %X\n", c);

	// 0~7 ��Ʈ ���� set
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