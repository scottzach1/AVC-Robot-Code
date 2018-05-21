#include <iostream>
#include <stdio.h>
#include "E101.h"

double kd = 0, kp = 0.4;
int motorSpeed = 70;

int getThreshold() {
    take_picture();
    /*
     * 320 * 240
     * (0, 1, 2, 3)
     * red, green, blue, luminosity (r + g + n)/3
    */
    int max = 0;
    int min = 255;
    char currentPixel;
    for (int i=0; i<320; i++) {
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

int getProportionalError(int thr) {
    int proportionalError, numWhitePixels = 0;
    for (int i = 0; i < 320; i++) {
        if (get_pixel(120, (320 - i), 3) > thr) {
            proportionalError += (i -
                                  160); // * get_pixel(120, (320-i), 3); // (320-0=i) camera upside-down // (i - 160) set middle at 0
            numWhitePixels++;
        }
    }
    if (numWhitePixels < 1) { // driveBackwards for 1 second
        set_motor(1, 60); // Revserse motors
        set_motor(2, 60);
        sleep1(1, 0);
        set_motor(1, 0);
        set_motor(1, 0);
        return 0;
    } else {
        return kp * (proportionalError / numWhitePixels);
    }
}

double getDerivativeError(int currentError, int previousError) {
    return kd * (currentError-previousError);
}

int main() {
    init();
    try{
        int currentError = 0, previousError = 0;
        int thr = getThreshold(), errorSignal = 0;
        while(true) {
            take_picture();
            errorSignal = getProportionalError(thr);
            set_motor(1, motorSpeed + error); // left
            set_motor(2, motorSpeed - error); // right
        }

    }catch(long e){ // Catch cntrl+c bug leaving robot motors fixed in an on state.
        printf("Caught error %f", e);
        set_motor(1, 0);
        set_motor(2, 0);
    }
    return 0;
}
