#include <iostream>
#include <stdio.h>
#include "E101.h"

// main structure https://github.com/kaiwhata/ENGR101-2017/wiki/PID-(Proportional-Integral-Derivative)-Control
double kd = 0;
double kp = 0.1;
int previousError = 0;
int leftDrive = 100;
int rightDrive = 100;

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
	int numWhitePixels = 0;
    for (int i=0; i<320; i++){
		if (get_pixel(120, (320-i), 3) > thr) {
			proportionalError += (i - 160);// * get_pixel(120, (320-i), 3); // (320-0=i) camera upside-down // (i - 160) set middle at 0
			numWhitePixels++;
		}
    }
    if(numWhitePixels<1) {
	return 100000000;
    }
    else {
    //printf("%d\n", proportionalError);
        return proportionalError / numWhitePixels;
    }
}

void derivativeError(int thr) {
    take_picture();
    int currentError = proportionalError(thr);
    double  derivativeError = kd * (currentError-previousError);
    double change = 0;//currentError * kp) - derivativeError;
    leftDrive = 70 + change;
    rightDrive = 70 - change;
    printf("der %f cur %d cha %f\n", derivativeError, currentError, change);
    printf("left %d right %d change %f\n", leftDrive, rightDrive, change);
    previousError = currentError;
}

int main() {
    init();
    try{
		int thr = getThreshold();
		printf("L %d R %d\n", leftDrive, rightDrive);
		while(true) {
			take_picture();
			derivativeError(thr);
			set_motor(1,-leftDrive);
			set_motormpu(2,-rightDrive);
		}
    
}catch(long meep){
	set_
}
    return 0;
}
