/*
 * author: Michael Scholten
 * Date: 7-5-2021
 * Description: reads the button to select a certain mode
 * modes: standby, ?, ?
*/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

// define the button pins
#define A_BUTTON (1 << PORTB3)
#define B_BUTTON (1 << PORTD5)
#define C_BUTTON (1 << PORTB0)

// define the Yellow LED pin
#define YELLOW_LED (1 << PORTC7)

void standby(){
    // turn every pin we don't need low
    PORTB &= ~C_BUTTON;
    PORTD &= ~B_BUTTON;
    PORTC &= ~YELLOW_LED;

    // wait until button is released
    while(~PINB & A_BUTTON);

    // wait until button is pressed again
    while(PINB & A_BUTTON);

    // wait until button is released again (prevents the device from directly returning to standby mode)
    while(~PINB & A_BUTTON);

    // activate the pull-up resistors from the button pins again
    PORTB |= C_BUTTON;
    PORTD |= B_BUTTON;
}

int main(){
    uint8_t i = 1;
    // make the button pins, input pins
    DDRB &= ~(A_BUTTON | C_BUTTON);
    DDRD &= ~B_BUTTON;

    // activate the pull-up resistors of the buttons
    PORTB |= A_BUTTON | C_BUTTON;
    PORTD |= B_BUTTON;

    // make the pin of the yellow LED, an output pin
    DDRC |= YELLOW_LED;

    while(1){
        // put the robot in standby mode, if the A button is pressed
        if(~PINB & A_BUTTON) standby();

        // if i == 0 (every 256 iterations = ~256 milliseconds), toggle the pin of the yellow led (~4 hz)
        if(i == 0){
            PORTC ^= YELLOW_LED;
        }

        // increment i
        i++;

        // wait a millisecond
        _delay_ms(1);
    }
}
