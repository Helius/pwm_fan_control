#include <stdio.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <stdlib.h>

#include <uart.h>
#include <ds18S20.h>

#define PERIOD 650

#define LOW_DEAD_ZONE 80
#define ADJ_WIDTH (520-LOW_DEAD_ZONE)

TSDS18x20 DS18x20;
TSDS18x20 *pDS18x20 = &DS18x20;

uint8_t idle_count = 0;
uint8_t speed = 5;

void togglePin(void)
{
	// toggle pin
	PORTB ^= (1 << PIN5);
}

void adjust_fan() {
		if (speed != 0) {
			OCR1A = LOW_DEAD_ZONE + (speed * (ADJ_WIDTH/10));
		} else {
			OCR1A = 1;
		}
//		printf("got %d -> %d\n\r", speed, OCR1A);
}

// receive one ascii char '0'-'9'
void uart_rx_handler(uint8_t ch)
{
	if (ch >= '0' && ch <= '9') {
		speed = ch - 0x30;
		adjust_fan();
		idle_count = 0;
	}
}

void pwm_init()
{
	DDRB |= (1 << PIN1);
  // Timer 1
  TCCR1A = (1 << COM1A1) | (1 << WGM11); // non-inverting, fast PWM
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10); // fast PWM, full speed
  
  ICR1 = PERIOD;         // 25 kHz from 16 MHz clock
  OCR1A = PERIOD;      // 50 % strobe
}

void do_magic(uint8_t t)
{
		printf("[%d]%d\n\r", speed, t);

		//int8_t k = t-42;
		//if (k > 0) {
		//	speed = k*2;
		//}
		//adjust_fan();
}

int main(void)
{
	pDS18x20->SensorModel = DS18B20Sensor;
	
	uart_init();
  set_receive_interrupt_handler(&uart_rx_handler);
	//printf("Start fan control unit!\n\r");
	
	DDRB |= (1 << PIN5);
  PORTD = (1 << PIN3);

	pwm_init();

	togglePin();
	
 while(DS18x20_Init(pDS18x20, &PORTD, PD5)) {
		//printf("Finding ds18b20!\n\r");
		_delay_ms(1000);
	} 
	
	printf("ds ok.\n\r");

	sei();

	while(1)
	{
		unsigned int temprx16;

		idle_count++;
		if(idle_count > 60) {
			speed = 5;
			adjust_fan();
		}

		togglePin();
		_delay_ms(1000);

		if (DS18x20_MeasureTemperature(pDS18x20))
		{
			temprx16 = DS18x20_TemperatureValue(pDS18x20);
			do_magic(temprx16/16);
		}
	}
}

