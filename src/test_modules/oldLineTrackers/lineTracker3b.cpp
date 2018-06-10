#include <iostream>
#include <stdio.h>
#include "E101.h"

int quad = 1;
double kp = 0.40;
int motorSpeed = -50;

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
    quad = 2;
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
    set_motor(1, 0);
    set_motor(2, -80);
    sleep1(0, 700000);
    set_motor(1, 0); // Reverse motors
    set_motor(2, 0);
}


void lineTracker() {
    take_picture();
    // getThreshold
    int max = 0;
    int min = 255;
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
    int thr = (max + min) / 2;
    printf("thr:%d\n", thr);

    while(quad == 2) {
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
        } else {
            proportionalSignal = kp * (currentError / numWhitePixels);
        }
        // Alter motor speeds
        printf("pSig: %d\n", proportionalSignal);
        set_motor(1, motorSpeed - proportionalSignal);
        set_motor(2, motorSpeed + proportionalSignal);
    }
    printf("Line tracked");
}

int main() {
    init();
    try {
        //openGate();
        //lineTracker();
        turnLeft();
    } catch (long e) {
        printf("Caught error %l", e);
        set_motor(1, 0);
        set_motor(2, 0);
    }
}
