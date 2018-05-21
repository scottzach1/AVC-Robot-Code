#include <iostream>
#include <stdio.h>
#include "E101.h"

int quad = 2;
double kp = 0.4, kd = 0.0;

void lineTracker() {
    int motorSpeed = 50;
    // getThreshold
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

    int thr = (max + min) / 2;
    int currentError = 0, previousError = 0;

    while(quad == 2) {
        // Calculate the current error
        int numWhitePixels = 0;
        for (int i = 0; i < 320; i++) {
            if (get_pixel(120, (320 - i), 3) > thr) {
                currentError += (i - 160); // * get_pixel(120, (320-i), 3); // (320-0=i) camera upside-down // (i - 160) set middle at 0
                numWhitePixels++;
            }
        }
        // Find the proportional signal
        double proportionalSignal = 0;
        if (numWhitePixels < 1) { // driveBackwards for 1 second
            set_motor(1, 60); // Reverse motors
            set_motor(2, 60);
            sleep1(1, 0);
            set_motor(1, 0);
            set_motor(1, 0);
        } else {
            proportionalSignal = kp * (currentError / numWhitePixels);
        }
        // Find the derivativeSignal
        double derivativeSignal = kd * (currentError - previousError);
        previousError = currentError;

        // Alter motor speeds
        set_motor(1, motorSpeed + proportionalSignal - derivativeSignal); // left
        set_motor(2, motorSpeed - proportionalSignal + derivativeSignal); // right
    }
}

int main() {
    init();
    try {
        lineTracker();
    } catch (long e) {
        printf("Caught error %l", e);
        set_motor(1, 0);
        set_motor(2, 0);
    }
}
