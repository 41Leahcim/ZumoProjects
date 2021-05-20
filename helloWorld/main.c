#include <avr/io.h>

#define RED_LED (1 << PORTB0)

void initUSART(){
    UCSR1A = 0;
    UCSR1B = (1 << TXEN1);
    UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
    UBRR1 = 103;
}

void sendChar(char c){
    while(~UCSR1A & (1 << UDRE1));
    UDR1 = c;
}

void sendString(const char *string, uint8_t length){
    uint8_t i = 0;
    while(i < length && string[i] != '\0') sendChar(string[i++]);
}

int main(){
    DDRB |= RED_LED;
    PORTB &= ~RED_LED;
    initUSART();
    while(1) sendString("Hello world!\r", 14);
}
