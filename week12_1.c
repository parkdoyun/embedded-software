#include <avr/io.h>  //ATmage128 �������� ����
#include <util/delay.h>
#include <avr/interrupt.h>#include <util/delay.h>#include <avr/interrupt.h>
#include <avr/io.h>  //ATmage128 �������� ����#include <util/delay.h>#include <avr/interrupt.h>
#define F_CPU 16000000UL

#define CDS_VALUE 871
#define SOL 96
#define ON 1
#define OFF 0

void init_adc();
unsigned short read_adc();
void show_adc(unsigned short value);
volatile int state = 0; // 0 : OFF, 1 : ON
int buzzer_on = 0; // 0 : OFF, 1 : ON
int cnt = 0;
int SOL_cnt = 784;
int on_off_cnt = 0; // 1���� �������� �︮�� �ϱ� ���� 

// �Ӻ�������Ʈ���� 12���� �ǽ�
// 12161569 ��ǻ�Ͱ��а� �ڵ���
// �ǽ����� 1 - ������ ���� �ǽ�����

ISR(TIMER0_OVF_vect){ // ���� ���� ��ȭ ���� �� ���� �ѹ� �︲
	if(buzzer_on == ON) // ���� �� �ִٸ�
	{
		if(state == ON)
		{
			PORTB = 0x00;
			state = OFF;
		}
		else
		{
			PORTB = 0x10;
			state = ON;
		}
		TCNT0 = SOL; // ��
		
		cnt++;
		if(cnt == SOL_cnt)
		{
			cnt = 0;
			_delay_ms(1000);
			buzzer_on = OFF; // ���� ����
			
		}
	}
}

unsigned char fnd_sel[4] = {0x01, 0x02, 0x04, 0x08};
unsigned char fnd_on[4] = {0x00, 0x3F, 0x37, 0x00};
unsigned char fnd_off[4] = {0x00, 0x3F, 0x71, 0x71};

int main(){
	unsigned short value;
	DDRA=0xff;
	DDRB = 0x10; // ���� ��� ����
	DDRC = 0xFF; // FND
	DDRG = 0x0F;

	TCCR0 = 0x03; // 32����
	TIMSK = 0x01; // overflow
	sei();
	init_adc();
	while(1){
		value=read_adc();
		show_adc(value);
		if(on_off_cnt == ON)
		{
			for(int i = 0; i < 4; i++) // FND 'ON'
			{
				PORTC = fnd_on[4-i];
				PORTG = fnd_sel[i];
				_delay_ms(2);
			}
		}
		else // OFF
		{
			for(int i = 0; i < 4; i++) // FND 'OFF'
			{
				PORTC = fnd_off[4-i];
				PORTG = fnd_sel[i];
				_delay_ms(2);
			}
		}
	}
}

void init_adc(){
	ADMUX=0x00;
		// 00000000
		// REFS(1:0) = "00" AREF(+5V) �������� ���
		// ADLAR = '0' ����Ʈ ������ ����
		// MUX(4:0) = "00000" ADC0 ���, �ܱ� �Է�
	ADCSRA = 0x87;
		// 10000111
		// ADEN = '1' ADC�� Enable
		// ADFR = '0' single conversion ���
		// ADPS(2:0) = "111" ���������Ϸ� 128����
}

unsigned short read_adc(){
	unsigned char adc_low,adc_high;
	unsigned short value;
	ADCSRA |= 0x40; // ADC start conversion, ADSC = '1'
	while((ADCSRA & (0x10)) != 0x10); // ADC ��ȯ �Ϸ� �˻�
	adc_low=ADCL;
	adc_high=ADCH;
	value = (adc_high <<8) | adc_low;
	
	return value;
}

void show_adc(unsigned short value){
	if(value < CDS_VALUE) {
		PORTA=0xAA;		// 10101010
		// ON
		if(on_off_cnt == OFF) // �������� OFF���ٸ� -> ���� ON
		{
			on_off_cnt = ON;
			buzzer_on = ON;
		}
		
	}
	else
	{
		PORTA=0x55;		// 01010101
		// OFF
		if(on_off_cnt == ON) // �������� ON���ٸ� -> ���� OFF
		{
			on_off_cnt = OFF;
			buzzer_on = ON;
		}
	}
}


