#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "geneticAlgorithm.h"

static void breed(){

}

static void mutate(){

}

void* geneticalgorithm(unsigned int iCount, unsigned int gCount, getInd newRand,
		processInd getFitness){
	//ensure that total fitness will never overflow
	//apparently gcc thinks it's impossible though
	//assert(UINT_MAX * iCount < ULONG_MAX);

	void **pop = malloc(sizeof(void*) * iCount);
	int *fitness = malloc(sizeof(int) * iCount);

	for (unsigned int x = 0; x < iCount; x++){
		pop[x] = newRand();
	}

	for (unsigned int g = 0; g < gCount; g++){
		for (unsigned int x = 0; x < iCount; x++){
			fitness[x] = getFitness(pop[x]);
		}

		breed();
		mutate();
	}

	return NULL;
}
