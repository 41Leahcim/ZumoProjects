/**
 * Name: Michael Scholten
 * Date: 20-5-2021
 * Description: Controlls the ZUMO robot from a distance using the proxity sensor and xbee
*/

#include <avr/interrupt.h>
#include "../library/proximitySensor.h"


void USART_init(){
    // clear UCSR1A
    UCSR1A = 0;

    // enable the USART receiver and receiver interrupt
    UCSR1B = (1 << RXEN1) | (1 << RXCIE1);

    // use 8 dat bits
    UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
}

ISR(USART1_RX_vect){
    // store the received char
    char c = UDR1;

    // stores the current speed
    static uint8_t speed = 0;

    if(c >= '0' && c <= '9'){
        speed = (c - '0') << 4;
    }else{
        c |= 32;
        if(c == 'w'){
        }else if(c == 's'){
        }else if(c == 'a'){
        }else if(c == 'd'){
        }else if(c == 'q'){
        }else if(c == 'e'){}
    }
}

int main(){}
