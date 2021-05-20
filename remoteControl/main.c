/**
 * Name: Michael Scholten
 * Date: 13-5-2021
 * Description: Gives full control over the robot using USART or XBee (XBee recommended)
*/
#include <avr/interrupt.h>
#include <limits.h>
#include "../library/motor.h"

ISR(USART1_RX_vect){
    // store the received char in variable c
    char c = UDR1;

    // stores the current speed
    static uint16_t speed = 0;
    // if the received char is a number ('0' - '9'), change the speed to c * 64
    if(c >= '0' && c <= '9'){
        speed = (c - '0') << 6;
        motors_setSpeedBoth(speed);
    }else{
        // convert the letter (if it is a valid char and wasn't a number), to lowercase
        c |= 32;
        
        // check which move to make and correct the speed
        if(c == 'w'){
            // if we received a W, go forward
            motors_setDirectionBoth(forward);
            motors_setSpeedBoth(speed);
        }
        else if(c == 's'){
            // if we received a S, go backward
            motors_setDirectionBoth(backward);
            motors_setSpeedBoth(speed);
        }
        else if(c == 'a'){
            // if we received a A, rotate left
            motors_rotateLeft();
            motors_setSpeedBoth(speed);
        }
        else if(c == 'd'){
            // if we received a D, rotate right
            motors_rotateRight();
            motors_setSpeedBoth(speed);
        }
        else if(c == 'q'){
            // if we received a Q, set the speed on the left side to half the current speed (forward/backward driving left rotation)
            motors_setSpeedLeft(motors_getSpeedLeft() >> 1);
            motors_setSpeedRight(speed);
        }
        else if(c == 'e'){
            // if we received a E, set the speed on the right side to half the current speed (forward/backward driving right rotation)
            motors_setSpeedRight(motors_getSpeedRight() >> 1);
            motors_setSpeedLeft(speed);
        }
    }
}

void initUSART(){
    // reset UCSR1A
    UCSR1A = 0;

    // enable the receiver and receiver interrupt
    UCSR1B = (1 << RXEN1) | (1 << RXCIE1);

    // 8 data bits
    UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);

    // baud rate 9600
    UBRR1 = 103;
}

int main(){
    // initialize the motors and USART
    motors_init();
    initUSART();
    
    // set the interrupts
    sei();

    while(1);
}
