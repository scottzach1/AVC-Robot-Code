#include <iostream>
#include <stdio.h>
#include "E101.h"

double kp = 0.32;
int motorSpeed = -45;
int max, min, thr;

void openGate() {
    char server_addr[15] = "130.195.6.196";
    int port = 1024;
    char message[24];
    char send[] = "Please";
    connect_to_server(server_addr, port);
    send_to_server(send);
    receive_from_server(message);
    send_to_server(message);
    printf("Gate Opened");
}

void reverse() {
    set_motor(1, 0);
    set_motor(2, 0);
    sleep1(0, 500000);
    set_motor(1, 60); // Reverse motors
    set_motor(2, 60);
    sleep1(0, 500000);
    set_motor(1, 0);
    set_motor(1, 0);
}
void turnLeft() {
    printf("TurnLeft\n");
    set_motor(1, 0);
    set_motor(2, -80);
    sleep1(0, 650000);
    set_motor(1, 0); // Reverse motors
    set_motor(2, 0);
}
void turnRight() {
    printf("TurnRight\n");
    set_motor(1, -70);
    set_motor(2, 0);
    sleep1(0, 700000);
    set_motor(1, 0); // Reverse motors
    set_motor(2, 0);
}
void getThr() {
    take_picture();
    // getThreshold
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
    thr = (max + min) / 2;
    printf("thr:%d\n", thr);
}

void lineTracker() {
    getThr();
    while(true) {
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
        //printf("pSig: %d wPx %d cEr\n", proportionalSignal, numWhitePixels, currentError);
        set_motor(1, motorSpeed - proportionalSignal);
        set_motor(2, motorSpeed + proportionalSignal);
    }
    printf("Line tracked\n");
}

void cornerTracker() {
    set_motor(1, -40);
    set_motor(2, -40);
    sleep1(1, 0);
    int corners = 0;
    while(true) {
        take_picture();
        // Calculate the current error
        int currentError = 0, numWhitePixels = 0;
        for (int i = 0; i < 320; i++) {
            if (get_pixel(120, (320 - i), 3) > thr) {
                currentError += (i - 160); // * get_pixel(120, (320-i), 3); // (320-0=i) camera upside-down // (i - 160) set middle at 0
                numWhitePixels++;
            }
        }
        printf("currentError: %d\n", currentError);
        // Find the proportional signal
        double proportionalSignal = 0;
        if (numWhitePixels < 1) { // driveBackwards for 1 second
            if (corners == 0) {
                 turnLeft();
                 corners = 1;
                 printf("Turned Left First Intersection\n");
            } else {
                 reverse();
            }
        } else if (numWhitePixels > 160) {
             if (currentError<-10000) {
                  turnLeft();
             } else if (currentError>1000) {
                  turnRight();
             } else {
                   proportionalSignal = kp * (currentError / numWhitePixels);
             }
        } else {
            proportionalSignal = kp * (currentError / numWhitePixels);
        }
        // Alter motor speeds
//        printf("pSig: %d\n", proportionalSignal);
        set_motor(1, motorSpeed - proportionalSignal);
        set_motor(2, motorSpeed + proportionalSignal);
    }
    printf("Line tracked\n");
}

int main() {
    init();
    try {
        //turnLeft();
        //turnRight();
        openGate();
        lineTracker();
        cornerTracker();
        set_motor(1, 0);
        set_motor(2, 0);
    } catch (long e) {
        printf("Caught error %l", e);
        set_motor(1, 0);
        set_motor(2, 0);
    }
}
