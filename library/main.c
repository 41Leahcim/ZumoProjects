#include "proximitySensor.h"
#include "motor.h"

int main(){
    motors_init();
    motors_rotateLeft();
    motors_rotateRight();
    motors_setDirectionBoth(0);
    motors_setDirectionLeft(0);
    motors_setDirectionRight(0);
    motors_setSpeedBoth(0);
    motors_setSpeedLeft(0);
    motors_setSpeedRight(0);

    SensorData data;
    uint16_t arr[] = {1, 2, 3};
    data.side = frontProxSensor;
    proxSensor_read(&data, 1, arr, 3, 1);
    return 0;
}
