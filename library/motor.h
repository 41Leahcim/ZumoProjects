/*
 * Author: Michael Scholten
 * Date: 8-5-2021
 * Description: helps the user to use the motors
 * sources:
 * https://www.pololu.com/docs/0J63/3.10
 * https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf
*/

// make sure this file is not included multiple times
#pragma once

#include <avr/io.h>

// define the motor pins
#define MOTOR_LEFT (1 << PORTB6)
#define MOTOR_RIGHT (1 << PORTB5)
#define MOTOR_BOTH (MOTOR_LEFT | MOTOR_RIGHT)

// define motor direction pins
#define MOTOR_DIRECTION_LEFT (1 << PORTB2)
#define MOTOR_DIRECTION_RIGHT (1 << PORTB1)
#define MOTOR_DIRECTION_BOTH (MOTOR_DIRECTION_LEFT | MOTOR_DIRECTION_RIGHT)

// has to be called before using the motors
inline void motors_init(){
    // make the motor control pins, output pins
    DDRB |= MOTOR_BOTH | MOTOR_DIRECTION_BOTH;
    PORTB &= ~(MOTOR_BOTH | MOTOR_DIRECTION_BOTH);

    // COM1A1, COM1B1: set at top, clear at compare match
    // WGM10, WGM11, WGM12: 10 bit Fast PWM
    // CS10: no prescaling
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10) | (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << CS10);
    OCR1A = 0;
    OCR1B = 0;
}

inline void motors_setSpeedRight(uint16_t speed){
    // sets the speed of the right motor to the value of the 10 least significant bits
    // this speed is the amount of electricity sent to the motor
    OCR1A = speed & 1023;
}

inline void motors_setSpeedLeft(uint16_t speed){
    // sets the speed of the left motor to the value of the 10 least significant bits
    // this speed is the amount of electricity sent to the motor
    OCR1B = speed & 1023;
}

inline void motors_setSpeedBoth(uint16_t speed){
    // make sure the speed is between 0 and 1023
    speed &= 1023;

    // sets both motors to the same given electricity level
    OCR1A = speed;
    OCR1B = speed;
}

// an enumeration to help choosing the correct value
typedef enum{ forward = 0, backward = 1 } Direction;

inline void motors_setDirectionLeft(Direction direct){
    // if the the backward direction is entered, set the left motor direction backwards
    // else, set it forwards
    if(direct == backward) PORTB |= MOTOR_DIRECTION_LEFT;
    else PORTB &= ~MOTOR_DIRECTION_LEFT;
}

inline void motors_setDirectionRight(Direction direct){
    // if the the backward direction is entered, set the right motor direction backwards
    // else, set it forwards
    if(direct == backward) PORTB |= MOTOR_DIRECTION_RIGHT;
    else PORTB &= ~MOTOR_DIRECTION_RIGHT;
}

inline void motors_setDirectionBoth(Direction direct){
    // if the the backward direction is entered, set the motors direction backwards
    // else, set it forwards
    if(direct == backward) PORTB |= MOTOR_DIRECTION_BOTH;
    else PORTB &= ~MOTOR_DIRECTION_BOTH;
}

inline void motors_rotateLeft(){
    // set the left motor direction backwards and the right motor direction forwards
    PORTB = (PORTB | MOTOR_DIRECTION_LEFT) & ~MOTOR_DIRECTION_RIGHT;
}

inline void motors_rotateRight(){
    // set the right motor direction backwards and the left motor direction forwards
    PORTB = (PORTB | MOTOR_DIRECTION_RIGHT) & ~MOTOR_DIRECTION_LEFT;
}

inline uint16_t motors_getSpeedLeft(){
    return OCR1B;
}

inline uint16_t motors_getSpeedRight(){
    return OCR1A;
}

#undef MOTOR_LEFT
#undef MOTOR_RIGHT
