#include <stdlib.h>
#include <limits.h>
#include <stdio.h>

#include "helper.h"

//RAND_MAX is at least RAND_MAX_MIN
static const int RAND_MAX_MIN = 32767;
//RAND_MAX_MIN is 2^15 - 1
static const int RAND_WIDTH = 15;

static int* initMasks(){
	int *masks = malloc(sizeof(int) * RAND_WIDTH);
	masks[0] = 0;
	masks[1] = 1;
	for (int x = 2; x < RAND_WIDTH; x++){
		masks[x] = masks[x-1] | (masks[x - 1] + 1);
	}
	return masks;
}

void printL(unsigned long l){
	for(int x = 0; x < longWidth; x++){
		printf("%u", (unsigned int) l%2);
		l = l >> 1;
	}
	printf("\n");
}

void printI(unsigned int i){
	for(int x = 0; x < intWidth; x++){
		printf("%u", i%2);
		i = i >> 1;
	}
	printf("\n");
}

unsigned long randl(){
	int runCount = sizeof(long) * CHAR_BIT / RAND_WIDTH;
	int remainder = sizeof(long) * CHAR_BIT / RAND_WIDTH;
	int* masks = initMasks();

	long l;
	int r;
	for (int x = 0; x < runCount; x++){
		l = l << RAND_WIDTH;
		r = rand() & 0x7FFF; //hopefully evenly distributed
		l = l | r;
	}
	l = l << remainder;
	r = rand() & masks[remainder];
	l = l | r;
	return l;
}

