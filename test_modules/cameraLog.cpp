
#include  <stdio.h>
#include  <time.h>
#include "E101.h"

void getThreshold(){
	take_picture(); // 320 * 240
	// 0 = red
	// 1 = green
	// 2 = blue
	// 3 = luminocity (r + g + n)/3
	int max = 0;
	int min = 255;
	int currentPixel = 0;
	for(int i=0; i<320; i++) {
		currentPixel = get_pixel(120, i, 3);
		if (currentPixel > max) {
			max = currentPixel;
		}
		if (currentPixel < min) {
			min = currentPixel;
		}
		fprintf(file, "%d ", currentPixel);
	}
	fprintf(file, "\n");
	printf("%d max %d min\n", max, min);
}

int main()
{

	init();
	FILE *file;
	file = fopen("log.txt", "w");

	for(int i=0; i<15; i++) {
		getThreshold();
		sleep1(1, 0);
	}
	fclose(file)
}
