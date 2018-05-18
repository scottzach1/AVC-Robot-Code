#include <iostream>
#include <stdio.h>
#include <time.h>
#include "E101.h"

int main() {
	init()
	set_motor(1, 100);
	set_motor(2, 100);
	sleep1(1, 0);
	set_motor(1, 0);
	set_motor(2, 0);
}
