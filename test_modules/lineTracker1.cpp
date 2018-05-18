#include <iostream>
#include <stdio.h>
#include <time.h>
#include "E101.h"

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

int getError() {
	take_picture();
	int error = 160;
	int totalError = 0;
	for (int i = 0; i < 320; i++){
		if(i==159) {
			error--;
		}		
		if(108 < (int)get_pixel(120, i, 3)){
			totalError += error;
		}
		error--;
	}
	return totalError;
}

int main(){
	init();
	int thr = getThreshold();
	for (int i=0; i<10; i++) { // Basic pivot trial. Will be experimenting with PID like system later.
		if (getError() < 0){
			set_motor(1,-255);
			set_motor(2,0);
		}
		else if (0 < getError()){
			set_motor(2, -255);
			set_motor(1,0);
		}
		else{
			set_motor(2, -255);
			set_motor(1,-255);
		}
		sleep1(1,0);
	}
	set_motor(1,0);
	set_motor(2,0);
	printf("%d", thr);
	return 0;
}