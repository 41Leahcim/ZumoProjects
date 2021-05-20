/**
 * Name: Michael Scholten
 * Date: 10-5-2021
 * Description: ROBOT DANCE!!!!
*/

#define F_CPU 16000000
#include <util/delay.h>

#include "../library/motor.h"

int main(){
    uint16_t speed = 0;
    uint8_t i, j;

    motors_init();

    for(j = 0;j < 2;j++){
        while(speed < 1023){
            motors_setSpeedBoth(speed);
            speed++;
            _delay_ms(1);
        }
        while(speed > 0){
            motors_setSpeedBoth(speed);
            speed--;
            _delay_ms(1);
        }

        motors_setDirectionBoth(backward);

        while(speed < 1022){
            motors_setSpeedBoth(speed);
            speed += 2;
            _delay_ms(1);
        }
        while(speed > 0){
            motors_setSpeedBoth(speed);
            speed -= 2;
            _delay_ms(1);
        }

        motors_rotateLeft();

        while(speed < 1023){
            motors_setSpeedBoth(speed);
            speed++;
            _delay_ms(2);
        }
        while(speed > 0){
            motors_setSpeedBoth(speed);
            speed--;
            _delay_ms(2);
        }

        motors_rotateRight();

        while(speed < 1023){
            motors_setSpeedBoth(speed);
            speed++;
            _delay_ms(2);
        }
        while(speed > 0){
            motors_setSpeedBoth(speed);
            speed--;
            _delay_ms(2);
        }

        motors_setDirectionBoth(forward);

        for(i = 0;i < 2;i++){
            while(speed < 1023){
                motors_setSpeedLeft(speed);
                motors_setSpeedRight(speed >> 1);
                speed += 2;
                _delay_ms(1);
            }

            while(speed > 0){
                motors_setSpeedLeft(speed);
                motors_setSpeedRight(speed >> 1);
                speed -= 2;
                _delay_ms(1);
            }

            while(speed < 1023){
                motors_setSpeedRight(speed);
                motors_setSpeedLeft(speed >> 1);
                speed += 2;
                _delay_ms(1);
            }

            while(speed > 0){
                motors_setSpeedRight(speed);
                motors_setSpeedLeft(speed >> 1);
                speed -= 2;
                _delay_ms(1);
            }

            while(speed < 1023){
                motors_setSpeedRight(speed);
                motors_setSpeedLeft(speed >> 1);
                speed += 2;
                _delay_ms(1);
            }

            while(speed > 0){
                motors_setSpeedRight(speed);
                motors_setSpeedLeft(speed >> 1);
                speed -= 2;
                _delay_ms(1);
            }

            while(speed < 1023){
                motors_setSpeedLeft(speed);
                motors_setSpeedRight(speed >> 1);
                speed += 2;
                _delay_ms(1);
            }

            while(speed > 0){
                motors_setSpeedLeft(speed);
                motors_setSpeedRight(speed >> 1);
                speed -= 2;
                _delay_ms(1);
            }
        }
        while(speed < 1023){
            motors_setSpeedLeft(speed);
            motors_setSpeedRight(speed >> 1);
            speed++;
            _delay_ms(2);
        }
        while(speed > 0){
            motors_setSpeedLeft(speed);
            motors_setSpeedRight(speed >> 1);
            speed--;
            _delay_ms(1);
        }
    }
    while(1){
    }
}
