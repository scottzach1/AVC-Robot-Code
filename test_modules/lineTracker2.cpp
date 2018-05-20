#include <iostream>
#include "E101.h"

// main structure https://github.com/kaiwhata/ENGR101-2017/wiki/PID-(Proportional-Integral-Derivative)-Control
int kd = 0.2;
int previousError = 0;
int leftDrive, rightDrive = 100;


int proportionalError() {
    int proportionalError = 0;
    for (int i=0; i<320; i++){
        proportionalError += (i - 160) * get_pixel(120, (320-i), 3); // (320-0=i) camera upside-down // (i - 160) set middle at 0
    }
    return proportionalError;
}

int derivativeError() {
    int currentError = 0;
    currentError = proportionalError();
    sleep1(0, 100000);
    int derivativeError = kd * (currentError-previousError);
    previousError = currentError;
    leftDrive += derivativeError;
    rightDrive -= derivativeError;
//    printf("%d\n", derivativeError);
}

int main() {
    init();
    while(true) {
        take_picture();
        derivativeError();
        set_motor(1, -leftDrive);
        set_motor(2, -rightDrive);
    }
}
