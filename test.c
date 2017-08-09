#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "helper.h"
#include "geneticAlgorithm.h"

//len of 10 or higher will overflow a 32 bit integer. However, Window's longs
//are also 32 bit, so it would be wise to set the max to 9 - log10(popSize)
static const int strLen = 9;

static inline char randDigit(){
	return '0' + rand() % 10;
}

static inline void overflowCheck(unsigned int popSize){
	if (strLen + log(popSize)/log(10) > log(ULONG_MAX) / log(10)){
		printf("ERROR: you machine's longs are not long enough to "
			"perform this test. Please decrease the value of "
			"strLen and recompile to proceed\n");
		exit (EXIT_FAILURE);
	}
}

void* randStr(){
	char *str = malloc(sizeof(char) * (strLen + 1));
	for (int x = 0; x<strLen; x++){
		str[x] = randDigit();
	}
	str[strLen] = '\0';
	return str;
}

unsigned int getFitness(void *str){
	return (unsigned int) atoi((char*)str);
}

void breedStr(void *s1, void *s2){
	char tmp[strLen - 1];
	int i = 1 + rand() % (strLen - 2);
	strncpy(tmp, s2, i);
	strncpy(s2, s1, i);
	strncpy(s1, tmp, i);
}

void* cloneStr(void *s1){
	char *s2 = malloc(sizeof(char) * (strLen+1));
	strcpy(s2, s1);
	return s2;
}

void mutateString(void *str){
	((char*)str)[rand() % strLen] = randDigit();
}

int main (int argc, char **argv){
	(void) argc;
	(void) argv;

	srand(time(NULL));

	geneticParams p = geneticParamsDefault();
	p.popSize = 4000;
	p.genCount = 1000;

	overflowCheck(p.popSize);
	char *str = geneticAlgorithm(p, randStr, getFitness, cloneStr,
				mutateString, breedStr);
	printf("The best string found was \"%s\"\n", str);
	free(str);
	return 0;
}
