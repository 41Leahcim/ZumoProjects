/**
 * Name: Michael Scholten
 * Date: 20-5-2021
 * Description: Controlls the ZUMO robot from a distance using the proxity sensor and xbee
*/

#include <avr/interrupt.h>
#include "../library/proximitySensor.h"
#include "../library/motor.h"

void USART_init(){
    // clear UCSR1A
    UCSR1A = 0;

    // enable the USART receiver and receiver interrupt
    UCSR1B = (1 << RXEN1) | (1 << RXCIE1);

    // use 8 dat bits
    UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);

    UBRR1 = 103;
}

// stores the current speed
volatile uint16_t speed = 0;

ISR(USART1_RX_vect){
    // store the received char
    char c = UDR1;
    
    if(c >= '0' && c <= '9'){
        speed = (c - '0') << 6;
        OCR1A = speed;
        OCR1B = speed;
    }else{
        c |= 32;
        if(c == 'w'){
            motors_setDirectionBoth(forward);
            OCR1A = speed;
            OCR1B = speed;
        }else if(c == 's'){
            motors_setDirectionBoth(backward);
            OCR1A = speed;
            OCR1B = speed;
        }else if(c == 'a'){
            motors_rotateLeft();
            OCR1A = speed;
            OCR1B = speed;
        }else if(c == 'd'){
            motors_rotateRight();
            OCR1A = speed;
            OCR1B = speed;
        }else if(c == 'q'){
            motors_setDirectionBoth(forward);
            OCR1A = speed;
            OCR1B >>= 1;
        }else if(c == 'e'){
            motors_setDirectionBoth(forward);
            OCR1A >>= speed;
            OCR1B = 1;
        }
    }
}

int main(){
    uint8_t stopStart = 0;
    motors_init();
    USART_init();
    sei();
    uint16_t brightness[] = {4, 15, 32, 55, 85, 120};
    SensorData frontSensor = {.side = frontProxSensor};
    while(1){
        proxSensor_read(&frontSensor, 1, brightness, sizeof(brightness) / sizeof(*brightness), 420);
        if((frontSensor.withRightLeds | frontSensor.withLeftLeds) == sizeof(brightness) / sizeof(*brightness) && ((~PORTB & MOTOR_DIRECTION_LEFT) && (~PORTB & MOTOR_DIRECTION_RIGHT))){
            stopStart++;
            if(stopStart > 1){
                stopStart = 0;
                OCR1A = 0;
                OCR1B = 0;
            }
        }else{
            stopStart--;
            if(stopStart < -1){
                stopStart = 0;
                OCR1A = speed;
                OCR1B = speed;
            }
        }
    }
}
