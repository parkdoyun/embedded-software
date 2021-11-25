/* GPIO�� LED �Ѱ� ���� 1�� ��
 LED 8���� ����Ǿ� �ִ� ��Ʈ : Port A(PA)
 - ��Ʈ7 : LED7(LD1), ��Ʈ6 : LED6(LD2)
 - ...
 - ��Ʈ1 : LED1(LD7), ��Ʈ0 : LED0(LD8) */

 // �Ӻ�������Ʈ���� 11���� �ǽ�
 // 12161569 ��ǻ�Ͱ��а� �ڵ���

 #include <avr/io.h>  // ATmage128 �������� ����
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

 // Buzzer ���� ���α׷� 1
 /*
 const int scale[8] = {DO, RE, MI, FA, SOL, LA, TI, UDO};
 int state = ON; // ���ļ� �߻���Ű�� ���� �Ҹ��� ���� �� ����
 int index = 0; // ���° ���� �߻���ų �� ������ ����
 int flag = 0; // ����ġ�� �ߺ����� ������ ���� �����ϱ� ���� ����

 ISR(TIMER0_OVF_vect){ // overflow
 	if(state == ON)
	{
		PORTB = 0x00; // state on -> buzzer ��°� 1 ����
		state = OFF; // change state
	}
	else
	{
		PORTB = 0x10; // state off -> buzzer ��°� 0 ����
		state = ON; // change state
	}
	TCNT0 = scale[index]; // ���� index�� �ش��ϴ� �Ҹ� ���
	flag = 1; // �ߺ� ������ �����ϱ� ���� �Ѵܰ� �� ��� �� flag ��ȯ
 }

 ISR(INT5_vect){ // interrupt 5
 	if(flag == 1) // flag 1�� ��쿡�� index 1 ����
	{	
		index = (index + 1) % 8; flag = 0; // ���� �� flag = 0���� ����
		_delay_ms(3000); // delay 3 sec
	}
 }

 int main()
 {
	// 1. ���� ������� ����
	DDRB = 0x10;
	// 2. ����ġ �Է� ���� -> SW2 enable
	DDRE = 0xDF; // 1101 1111
	// 3. interrupt 5 enable (0010 0000)
	EIMSK = 0x20; // 0010 0000
	// 4. �ϰ��������� ���ͷ�Ʈ �߻�
	EICRB = 0x08; // 0000 1000
	// 5. global interrupt Ȱ��ȭ
	sei();
	// 6. 32����
	TCCR0 = 0x03; // 0000 0011
	// 7. overflow �ǵ��� ����
	TIMSK = 0x01;
 	
 	for(;;)
	{
		
	}

	return 0;
 }
*/

// Buzzer ���� ���α׷� 2

const unsigned char melody[24] = {SOL, SOL, LA, LA, SOL, SOL, MI,
	SOL, SOL, MI, MI, RE, SOL, SOL, LA, LA, SOL, SOL, MI, SOL, MI, RE, MI, DO};
int index = 0;
int state = ON;
int cnt = 0;
// �� ��ε��� ������ �ʿ��� interval �迭
const int interval[24] = {2000, 2000, 2000, 2000, 2000, 2000, 5000, 2000, 2000, 2000, 2000, 5000,
	2000, 2000, 2000, 2000, 2000, 2000, 5000, 2000, 2000, 2000, 2000, 5000};

// �� ��ε��� ���ļ��� 2�� ���� �迭 -> ������ ���� �ð� ���� �Ҹ� ���� ���ؼ�
const int change[24] = {784, 784, 880, 880, 784, 784, 659, 784, 784, 659, 659, 587,
	784, 784, 880, 880, 784, 784, 659, 784, 659, 587, 659, 523};

ISR(TIMER0_OVF_vect){ // overflow
 	if(state == ON)
	{
		PORTB = 0x00; // state on -> buzzer ��°� 1 ����
		state = OFF; // change state
	}
	else
	{
		PORTB = 0x10; // state off -> buzzer ��°� 0 ����
		state = ON; // change state
	}
	TCNT0 = melody[index]; // ���� index�� �ش��ϴ� �Ҹ� ���
	cnt++;
	if(cnt == change[index]) // count �� ä���ٸ� (���� ��ε�� �Ѿ)
	{	
		cnt = 0; // �ʱ�ȭ
		_delay_ms(interval[index]); // delay
		index = (index + 1) % 24;
	}
 }

 int main()
 {	
	DDRB = 0x10; // ���� ��� ����
	TCCR0 = 0x03; // 32����, 0000 0011
	TIMSK = 0x01; // overflow
	sei(); // ���� ���ͷ�Ʈ enable

	while(1)
	{
		
	}
 	return 0;
 }

