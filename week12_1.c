#include <avr/io.h>  //ATmage128 레지스터 정의
#include <util/delay.h>
#include <avr/interrupt.h>#include <util/delay.h>#include <avr/interrupt.h>
#include <avr/io.h>  //ATmage128 레지스터 정의#include <util/delay.h>#include <avr/interrupt.h>
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
int on_off_cnt = 0; // 1번만 버저음이 울리게 하기 위해 

// 임베디드소프트웨어 12주차 실습
// 12161569 컴퓨터공학과 박도윤
// 실습과제 1 - 광센서 자유 실습과제

ISR(TIMER0_OVF_vect){ // 빛의 상태 변화 있을 때 버저 한번 울림
	if(buzzer_on == ON) // 버저 켜 있다면
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
		TCNT0 = SOL; // 솔
		
		cnt++;
		if(cnt == SOL_cnt)
		{
			cnt = 0;
			_delay_ms(1000);
			buzzer_on = OFF; // 버저 끄기
			
		}
	}
}

unsigned char fnd_sel[4] = {0x01, 0x02, 0x04, 0x08};
unsigned char fnd_on[4] = {0x00, 0x3F, 0x37, 0x00};
unsigned char fnd_off[4] = {0x00, 0x3F, 0x71, 0x71};

int main(){
	unsigned short value;
	DDRA=0xff;
	DDRB = 0x10; // 버저 출력 설정
	DDRC = 0xFF; // FND
	DDRG = 0x0F;

	TCCR0 = 0x03; // 32분주
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
		// REFS(1:0) = "00" AREF(+5V) 기준전압 사용
		// ADLAR = '0' 디폴트 오른쪽 정렬
		// MUX(4:0) = "00000" ADC0 사용, 단극 입력
	ADCSRA = 0x87;
		// 10000111
		// ADEN = '1' ADC를 Enable
		// ADFR = '0' single conversion 모드
		// ADPS(2:0) = "111" 프리스케일러 128분주
}

unsigned short read_adc(){
	unsigned char adc_low,adc_high;
	unsigned short value;
	ADCSRA |= 0x40; // ADC start conversion, ADSC = '1'
	while((ADCSRA & (0x10)) != 0x10); // ADC 변환 완료 검사
	adc_low=ADCL;
	adc_high=ADCH;
	value = (adc_high <<8) | adc_low;
	
	return value;
}

void show_adc(unsigned short value){
	if(value < CDS_VALUE) {
		PORTA=0xAA;		// 10101010
		// ON
		if(on_off_cnt == OFF) // 이전까지 OFF였다면 -> 버저 ON
		{
			on_off_cnt = ON;
			buzzer_on = ON;
		}
		
	}
	else
	{
		PORTA=0x55;		// 01010101
		// OFF
		if(on_off_cnt == ON) // 이전까지 ON였다면 -> 버저 OFF
		{
			on_off_cnt = OFF;
			buzzer_on = ON;
		}
	}
}


