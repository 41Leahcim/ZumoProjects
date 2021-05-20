/**
 * Author: Michael Scholten
 * Date: 9-5-2021
 * Description: helps user to use the proximity sensors
*/

// make sure this file is not included multiple times
#pragma once

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

// the pin to read the front proximity sensor
#define PROX_SENSOR_FRONT (1 << PORTF1)
#define PROX_SENSOR_LEFT (1 << PORTF5)
#define PROX_SENSOR_RIGHT (1 << PORTD4)

// the proximity LEDs
#define PROX_LED (1 << PORTC6)
#define PROX_LED_DIRECTION (1 << PORTF6)

#define PULSE_ON_US 421
#define PULSE_OFF_US 578

/***** the IR pulses part starts here *****/

enum Direction{ Left = 0, Right = 1};

// recommended period: 420
static void IRPulsesStart(enum Direction direction, uint16_t brightness, uint16_t period){
    // reset timer3
    TIMSK3 = 0;

    // brightness shouldn't be higher than period
    if(brightness > period){
        brightness = period;
    }

    // set the PWM pin to be an input. Otherwise, when we configure the COM3A bits, the signal might be high from previous activity and result
    // in a glitch
    PORTC &= ~PROX_LED;
    DDRC &= ~PROX_LED;

    // put the timer into a known state that shouldn't cause trouble while configuring it
    TCCR3A = (1 << COM3A1);
    TCCR3B = 0;

    // simulate a compare match, which makes the OC3A PWM signal (which is not connected) low. We must do dit after configuring COM3A
    TCCR3C = (1 << FOC3A);

    // make the PWM pin an output pin. The OC03A signal will controll its value
    DDRC |= PROX_LED;

    // Drive PORTF6 high or low to select which LEDs to use
    if(direction){
        // right
        PORTF |= PROX_LED_DIRECTION;
    }else{
        // left
        PORTF &= ~PROX_LED_DIRECTION;
    }
    // make PORTF6 an output pin
    DDRF |= PROX_LED_DIRECTION;

    // Set frequency/period
    ICR3 = period;

    // set the count to be one less thand ICR3 so that the new duty cycle will take effect very soon
    TCNT3 = period - 1;

    // set the duty cycle
    OCR3A = brightness;
    
    // start the timer, it will start running once the clock source bits in TCCR3B are set
    // set OC3A on match, clear at top
    // Fast PWM with ICR3 as top
    // Internal clock without prescaler
    TCCR3A = (1 << COM3A1) | (1 << WGM31);
    TCCR3B = (3 << WGM32) | (1 << CS30);
}

static void IRPulsesStop(){
    // Prepare the PWM pin to drive low.
    //We don't want to just set it as an input, because then it might decay from high to low gradually and the LEDs would not turn off immediately.
    PORTC &= ~PROX_LED;
    DDRC |= PROX_LED;

    // Disconnect the PWM signal from the pin, causing it to drive low. We must do this before stopping the timer to avoid glitches
    TCCR3A = (1 << WGM31);

    // turn off the timer
    TCCR3B = 0;

    // restore the timer's default settings to help avoid compatibility issues with libraries
    TIMSK3 = 0;
    TCCR3A = 0;
    OCR3A = 0;
    ICR3 = 0;
    TCNT3 = 0;
    
    // change the IR LED direction pin back to an input so it can be used for measuring the battery level
    DDRF &= ~PROX_LED_DIRECTION;
    PORTF &= ~PROX_LED_DIRECTION;
}

/***** end of IR pulses *****/
/***** start of proximity Sensor *****/

typedef enum{ frontProxSensor = PROX_SENSOR_FRONT, leftProxSensor = PROX_SENSOR_LEFT, rightProxSensor = PROX_SENSOR_RIGHT} ProxSensorSide;

// stores the sensor data
typedef struct{
    uint8_t withLeftLeds, withRightLeds;
    ProxSensorSide side;
} SensorData;

inline static uint8_t proxSensor_measure(enum Direction lightSide,  uint16_t brightness, uint16_t period, ProxSensorSide senseSide){
    uint8_t result = 0;

    // start infra red pulses at the left side
    IRPulsesStart(lightSide, brightness, period);

    // wait PULSE_ON_US microsecconds
    _delay_us(PULSE_ON_US);

    // check whether the front prox sensor pin is low
    switch(senseSide){
        case frontProxSensor:
            if(~PINF & senseSide) result = 1;
            break;
        case leftProxSensor:
            if(~PINF & senseSide) result = 1;
            break;
        case rightProxSensor:
            if(~PIND & senseSide) result = 1;
            break;
    }

    // stop sending pulses
    IRPulsesStop();

    // wait PULSE_OFF_US microsecconds
    _delay_us(PULSE_OFF_US);

    // return the result of the measurement
    return result;
}

void proxSensor_read(SensorData *dataStores, uint8_t numDataStores, uint16_t *brightnessLevels, uint8_t numLevels, uint16_t period){
    // declare the iteration variables
    uint8_t i, j;

    // test for every datas  store
    for(j = 0;j < numDataStores;j++){
        // select the current dataStore
        SensorData *dataStore = dataStores + j;

        // reset the sensed values for the left and right LEDs
        dataStore->withLeftLeds = 0;
        dataStore->withRightLeds = 0;

        // test every brightness level
        for(i = 0;i < numLevels;i++){
            // get the current brightness level
            uint16_t brightness = brightnessLevels[i];

            // take a measurement using the proximity sensor and left infra red lights
            dataStore->withLeftLeds += proxSensor_measure(Left, brightness, period, dataStore->side);

            // take a measurement using the proximity sensor and right infra red lights
            dataStore->withRightLeds += proxSensor_measure(Right, brightness, period, dataStore->side);
        }
    }
}
