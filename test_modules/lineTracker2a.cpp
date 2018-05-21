#include <iostream>
#include <stdio.h>
#include "E101.h"

// main structure https://github.com/kaiwhata/ENGR101-2017/wiki/PID-(Proportional-Integral-Derivative)-Control
double kd = 0.005;
int previousError = 0;
int leftDrive, rightDrive = 100;

int getThreshold(){
	take_picture(); // 320 * 240
	//int values = int[320];
	// 0 = red
	// 1 = green
	// 2 = blue
	// 3 = luminocity (r + g + n)/3
	int max = 0;
	int min = 255;
	char currentPixel;
	for(int i=0; i<320; i++) {
		currentPixel = get_pixel(120, i, 3);
		if (currentPixel > max) {
			max = currentPixel;
		}
		if (currentPixel < min) {
			min = currentPixel;
		}
	}
	return (max + min) / 2; // Should return a rounded int whole number.
}

int proportionalError(int thr) {
    int proportionalError = 0;
    for (int i=0; i<320; i++){
		if (get_pixel(120, (320-i), 3) > thr) {
			proportionalError += (i - 160);// * get_pixel(120, (320-i), 3); // (320-0=i) camera upside-down // (i - 160) set middle at 0
		}
    }
    //printf("%d\n", proportionalError);
    return proportionalError;
}

void derivativeError(int thr) {
    take_picture();
    int currentError = proportionalError(thr);
    sleep1(0, 100000);
    double  derivativeError = kd * (currentError-previousError);
    leftDrive -= derivativeError;
    rightDrive += derivativeError;
    printf("thr %d der %f left %d right %d\n", thr, derivativeError, leftDrive, rightDrive);
    previousError = currentError;
}

int main() {
    init();
    int thr = getThreshold();
    while(true) {
        take_picture();
        derivativeError(thr);
        set_motor(1, -leftDrive);
        set_motor(2, -rightDrive);
    }
    return 0;
}
