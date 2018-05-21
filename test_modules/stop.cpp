#include <iostream>
#include <stdio.h>
#include <time.h>
#include "E101.h"

int main() {
	init();
	set_motor(1, 0); // Left motor forward full duty cycle
	set_motor(2, 0); // Right motor forward full duty cycle
	sleep1(1, 0);
	set_motor(1, 0);
	set_motor(2, 0);
}
