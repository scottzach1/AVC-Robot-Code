#include <iostream>
#include <stdio.h>
#include "E101.h"

double kp = 0.28;
int motorSpeed = -55;
int max, min, thr;
FILE *file;

void openGate() {
    char server_addr[15] = "130.195.6.196";
    int port = 1024;
    char message[24];
    char send[] = "Please";
    connect_to_server(server_addr, port);
    send_to_server(send);
    receive_from_server(message);
    send_to_server(message);
    fprintf(file, "Gate Opened\n");
}

void stop() {
    set_motor(1, 0);
    set_motor(2, 0);
}

void reverse() {
    set_motor(1, 0);
    set_motor(2, 0);
    sleep1(0, 500000);
    set_motor(1, 60); // Reverse motors
    set_motor(2, 60);
    sleep1(0, 250000);
    stop();
}

void turnLeft() {
    fprintf(file, "TurnLeft\n");
    take_picture();
    stop();
    sleep1(0, 0);
    // Calculate the current error
    int currentError = 0, numWhitePixels = 0;
    double proportionalSignal = 100000;

    while (numWhitePixels < 50 && (proportionalSignal > 50 || proportionalSignal < -50)) {
        take_picture();
        set_motor(1, 50);
        set_motor(2, -50);
        sleep1(0, 500000);
        currentError = 0, numWhitePixels = 0, proportionalSignal = 0;
        for (int i = 0; i < 320; i++) {
            if (get_pixel(120, i+1, 3) > thr) {
                currentError += (i - 160);
                numWhitePixels++;
            }
        }
        if (numWhitePixels > 0) {
            proportionalSignal = kp * (currentError / numWhitePixels);
        }
    }
    stop();
    fprintf(file, "Stopped turning Left\n");
    fprintf(file, "exited with a wPx %d>50 propP -50<%f<50\n", numWhitePixels, proportionalSignal);
}

void turnRight() {
    fprintf(file, "RightLeft\n");
    take_picture();
    stop();
    // Calculate the current error
    int currentError = 0, numWhitePixels = 0;
    double proportionalSignal = 100000;

    while (numWhitePixels < 50 && (proportionalSignal > 50 || proportionalSignal < -50)) {
        take_picture();
        set_motor(1, -50);
        set_motor(2, 50);
        sleep1(0, 500000);
        currentError = 0, numWhitePixels = 0, proportionalSignal = 0;
        for (int i = 0; i < 320; i++) {
            if (get_pixel(120, i, 3) > thr) {
                currentError += (i - 160);
                numWhitePixels++;
            }
        }
        if (numWhitePixels > 0) {
            proportionalSignal = kp * (currentError / numWhitePixels);
        }
    }
    stop();
    fprintf(file, "Stopped turning Right\n");
    fprintf(file, "exited with a wPx %d>50 propP -50<%f<50\n", numWhitePixels, proportionalSignal);
}

int getThr() {
    take_picture();
    max = 0;
    min = 255;
    char currentPixel;
    for (int i = 0; i < 320; i++) {
        currentPixel = get_pixel(120, i, 3);
        if (currentPixel > max) {
            max = currentPixel;
        }
        if (currentPixel < min) {
            min = currentPixel;
        }
    }
    int var = (max + min) / 2;
    fprintf(file, "thr:%d\n", var);
    return var;
}

void lineTracker() {
    while(true) {
        thr = getThr();
        take_picture();
        // Calculate the current error
        int currentError = 0, numWhitePixels = 0;
        for (int i = 0; i < 320; i++) {
            if (get_pixel(120, (320 - i), 3) > thr) {
                currentError += (i - 160); // * get_pixel(120, (320-i), 3); // (320-0=i) camera upside-down // (i - 160) set middle at 0
                numWhitePixels++;
            }
        }
        // Find the proportional signal
        double proportionalSignal = 0;
        if (numWhitePixels < 1) { // driveBackwards for 1 second
            reverse();
        } else if (numWhitePixels > 310) {
            break;
        } else {
            proportionalSignal = kp * (currentError / numWhitePixels);
        }
        // Alter motor speeds
        //fprintf(file, "pSig: %d wPx %d cEr\n", proportionalSignal, numWhitePixels, currentError);
        set_motor(1, motorSpeed - proportionalSignal);
        set_motor(2, motorSpeed + proportionalSignal);
    }
    fprintf(file, "Line tracked\n");
}

void cornerTracker() {
    while (true) {
        take_picture();
        // Calculate the current error
        int currentError = 0, numWhitePixels = 0, numRedPixels = 0;
        for (int i = 0; i < 320; i++) {
            int white = get_pixel(120, (320 - i), 3);
            int red = get_pixel(120, (320 - i), 0);
            // * get_pixel(120, (320-i), 3); // (320-0=i) camera upside-down // (i - 160) set middle at 0
            if (white > thr) {
                currentError += (i - 160);
                numWhitePixels++;
            }
            if (red > white + 50) {
                numRedPixels++;
            }
        }
//        fprintf(file, "#thr: %d, wPx: %d\n", thr, numWhitePixels);
        // Find the proportional signal
        double proportionalSignal = 0;
        if (numRedPixels > 100) {
            fprintf(file, "So much redness, %d\n", numRedPixels);
            break;
        } else if (numWhitePixels < 15) { // driveBackwards for 1 second
            fprintf(file, "Corner Deteccc\n");
            stop();
            take_picture();
            // Check Left Right
            int leftCount = 0, rightCount = 0;
            for (int i = 0; i < 240; i++) {
                int left = get_pixel(i, 310, 3);
                int right = get_pixel(i, 10, 3);
                if (left > thr) {
                    leftCount++;
                }
                if (right > thr) {
                    rightCount++;
                }
            }
            fprintf(file, "leftCount: %d, rightCount: %d\n", leftCount, rightCount);
            if (leftCount >= 30) {
                fprintf(file, "lCount %d, rCount %d numWhite %d\n", leftCount, rightCount, numWhitePixels);
                fprintf(file, "Turn Left()\n");
                turnLeft();
            } else if (rightCount >= 30) {
                fprintf(file, "lCount %d, rCount %d numWhite %d\n", leftCount, rightCount, numWhitePixels);
                fprintf(file, "Turn Right()\n");
                turnRight();
            } else {
                reverse();
            }
        } else {
            proportionalSignal = (kp + 0.2) * (currentError / numWhitePixels);

            // Alter motor speeds
//        fprintf(file, "pSig: %d\n", proportionalSignal);
            set_motor(1, motorSpeed - proportionalSignal);
            set_motor(2, motorSpeed + proportionalSignal);
        }
    }
    fprintf(file, "Line tracked\n");
    printf("Line tracked\n");
}

int main() {
    init();
    file = fopen("log.txt", "w");
    printf("Log file created\n");
    try {
        thr = getThr();
        openGate();
        lineTracker();
        cornerTracker();
        stop();
    } catch (long e) {
        printf("Caught error %ld\n", e);
        stop();
    }
}