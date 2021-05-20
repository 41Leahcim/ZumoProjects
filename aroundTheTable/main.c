/*
 * main.c
 *
 * Created: 3-5-2021 17:11:44
 * Author : Michael Scholten
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

// define the motor (speed) pins
#define MOTOR_LEFT (1 << PORTB6)
#define MOTOR_RIGHT (1 << PORTB5)
#define MOTOR_BOTH (MOTOR_LEFT | MOTOR_RIGHT)

// define the motor direction pins
#define MOTOR_DIRECTION_LEFT (1 << PORTB2)
#define MOTOR_DIRECTION_RIGHT (1 << PORTB1)
#define MOTOR_DIRECTION_BOTH (MOTOR_DIRECTION_LEFT | MOTOR_DIRECTION_RIGHT)

#define DELAY 50

void turnRight(){
	while(OCR1B > 81){
		_delay_ms(DELAY);
		OCR1B--;
	}
	while(OCR1B < OCR1A + 8){
		_delay_ms(DELAY);
		OCR1B++;
	}
}

int main(void){
	DDRB |= MOTOR_BOTH;// make the pins of the motors, output pins

	// fast PWM for both motors, clear on compare match and set at TOP (255)
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
	TCCR1B = (1 << WGM12) | (1 << CS10);

	// make both motors on 32/255 of the time
	OCR1A = 1;
	OCR1B = 1;

	while(OCR1A < 120){
		_delay_ms(DELAY);
		OCR1A++;
		OCR1B = OCR1A + 8;
	}
	while(1){
		turnRight();
		turnRight();
		_delay_ms(DELAY * 35);
		turnRight();
		_delay_ms(DELAY * 20);
		turnRight();
		_delay_ms(DELAY * 25);
	}
}
