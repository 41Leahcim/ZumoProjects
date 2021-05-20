/**
 * Name: Michael Scholten
 * Date: 20-5-2021
 * Description: detects an object using the IR sensors
*/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include "../library/proximitySensor.h"

#define MOTOR_LEFT (1 << PORTB6)
#define MOTOR_RIGHT (1 << PORTB5)
#define MOTOR_BOTH (MOTOR_LEFT | MOTOR_RIGHT)

#define MOTOR_DIRECTION_LEFT (1 << PORTB2)
#define MOTOR_DIRECTION_RIGHT (1 << PORTB1)
#define MOTOR_DIRECTION_BOTH (MOTOR_DIRECTION_LEFT | MOTOR_DIRECTION_RIGHT)

#define YELLOW_LED (1 << PORTC7)

void motors_init(){
    DDRB |= MOTOR_DIRECTION_BOTH | MOTOR_BOTH;

    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B = (1 << WGM12) | (1 << CS10);
    OCR1A = 0;
    OCR1B = 0;
}

int main(){
    DDRC |= YELLOW_LED;
    motors_init();
    uint16_t brightness[] = {1, 2, 4, 8, 16, 32, 64, 128};
    SensorData sensors[] = {{.side=PROX_SENSOR_LEFT}, {.side=PROX_SENSOR_FRONT}, {.side=PROX_SENSOR_RIGHT}};
    while(1){
        proxSensor_read(sensors, sizeof(sensors) / sizeof(*sensors), brightness, sizeof(brightness) / sizeof(*brightness), 420);
        if(sensors[0].withLeftLeds - sensors[2].withRightLeds > 1){
            PORTB = (PORTB & ~MOTOR_DIRECTION_RIGHT) | MOTOR_DIRECTION_LEFT;
            OCR1A = 100;
            OCR1B = 100;
        }else if(sensors[2].withRightLeds - sensors[0].withLeftLeds > 1){
            PORTB = (PORTB & ~MOTOR_DIRECTION_LEFT) | MOTOR_DIRECTION_RIGHT;
            OCR1A = 100;
            OCR1B = 100;
        }else{
            OCR1A = 0;
            OCR1B = 0;
        }
    }
}
