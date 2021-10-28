/* GPIO�� LED �Ѱ� ���� 1�� ��
 LED 8���� ����Ǿ� �ִ� ��Ʈ : Port A(PA)
 - ��Ʈ7 : LED7(LD1), ��Ʈ6 : LED6(LD2)
 - ...
 - ��Ʈ1 : LED1(LD7), ��Ʈ0 : LED0(LD8) */

 // �Ӻ�������Ʈ���� 9���� �ǽ�
 // 12161569 ��ǻ�Ͱ��а� �ڵ���

 #include <avr/io.h>  // ATmage128 �������� ����
 #include <util/delay.h> // delay
 #define F_CPU 16000000UL

// LED �ǽ� ����
// �¿�� LED �̵�
/*
 void main(){
 	unsigned char value = 1;
 	DDRA= 0xff; // ��Ʈ A�� ��� ��Ʈ�� ���
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
 
 // LED ���� ����

void main(){
 	// �Ǻ��� �Ǵ� �迭 (�б� ���� ������)
	// LED�� ���ʺ��� �������ļֶ�õ��� ����
	// �ֶּ�� �ֹּ� �̷ֹּ̹���
	// �ֶּ�� �ֹּ� �̷ֹ��̵���
	unsigned char music[28] = {0x08, 0x08, 0x04, 0x04, 0x08, 0x08, 0x20, 0x20, 0x08, 0x08, 0x20, 0x20, 0x40, 0x40,
	0x08, 0x08, 0x04, 0x04, 0x08, 0x08, 0x20, 0x20, 0x08, 0x20, 0x40, 0x20, 0x80, 0x80};
	
	DDRA = 0xFF;
	int idx = 0; // �Ǻ��� �ε���
	for(;;)
	{
		PORTA = music[idx];
		_delay_ms(5000); // delay
		idx = (idx + 1) % 28; // index
	}
}

