#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "helper.h"
#include "geneticAlgorithm.h"

static const int strLen = 9;

static inline char randDigit(){
	return '0' + rand() % 10;
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

	char *str = (char*) geneticAlgorithm(100, 100, randStr, getFitness,
					breedStr, cloneStr, 0.5,
					mutateString, 0.05);
	free(str);
	return 0;
}
