#include <iostream>
#include <stdio.h>
#include "E101.h"

// main structure https://github.com/kaiwhata/ENGR101-2017/wiki/PID-(Proportional-Integral-Derivative)-Control
int kd = 0.2;
int kp = 0.5;
int previousError = 0;
int leftDrive, rightDrive = 0;


int proportionalError() {
    int proportionalError = 0;
    for (int i=0; i<320; i++){
        proportionalError += (i - 160) * get_pixel(120, (320-i), 3); // (320-0=i) camera upside-down // (i - 160) set middle at 0
    }
   // printf("%d\n", proportionalError);
    return proportionalError;
}

void derivativeError() {
    int currentError = proportionalError();
    int derivativeError = (currentError-previousError);// * kd
    previousError = currentError;
    //leftDrive += derivativeError;
    //rightDrive -= derivativeError;
    printf("cur %d pre %d der %d\n", currentError, previousError, derivativeError);
}

int main() {
    init();
    while(true) {
        take_picture();
        derivativeError();
        set_motor(1, -leftDrive);
        set_motor(2, -rightDrive);
    }
    return 0;
}
