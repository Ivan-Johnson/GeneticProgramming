#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "geneticAlgorithm.h"
#include "helper.h"

static const double SKEW_FACTOR = 0.1;

static int getRandomWeighted(unsigned int *fitness, int iCount,
			unsigned long totalFit){
	unsigned long randomFit;
	if ((unsigned long) iCount < (unsigned long) (SKEW_FACTOR * ULONG_MAX)){
		randomFit = randl() % totalFit;
	} else {
		do {
			randomFit = randl();
		} while (randomFit >= totalFit);
	}

	int i = 0;

	while (randomFit >= fitness[i]){
		assert(i < iCount);
		randomFit -= fitness[i];
		i++;
	}

	return i;
}

static void reproduce(void **pop, int iCount, cloner clone,
		unsigned int *fitness, unsigned long totalFit,
		breeder breed, double breedRatio,
		mutator mutate, double mutateRatio){
	if (breed == NULL){
		breedRatio = 0;
	}
	if (mutate == NULL){
		mutateRatio = 0;
	}
	int breedCount = (int) (iCount * breedRatio);
	int mutateCount = (int) (iCount * mutateRatio);
	assert(breedCount + mutateCount <= iCount);

	void **popWorking = malloc(sizeof(void*) * iCount);

	//create the next generation
	for (int x = 0; x < iCount; x++){
		int i = getRandomWeighted(fitness, iCount, totalFit);
		popWorking[x] = clone(pop[i]);
	}

	//breed the next generation
	if (breedCount % 2){//breed the odd child, if any
		breedCount--;
		unsigned tmpI = getRandomWeighted(fitness, iCount, totalFit);
		void *tmp = clone(pop[tmpI]);
		breed(tmp, popWorking[breedCount]);
		free(tmp);
	}
	for (int x = 0; x < breedCount; x += 2){
		breed(popWorking[x], popWorking[x+1]);
	}

	mutateCount += breedCount;
	for (int x = breedCount; x < mutateCount; x++){
		mutate(popWorking[x]);
	}

	//cleanup
	for (int x = 0; x < iCount; x++){
		free(pop[x]);
		pop[x] = popWorking[x];
	}
	free(popWorking);
}

static unsigned long computeFitness(void **pop, unsigned int *fitness,
				processInd getFitness, unsigned int iCount){
	unsigned long totalFit = 0;
	for (unsigned int x = 0; x < iCount; x++){
		fitness[x] = getFitness(pop[x]);
		totalFit += fitness[x];
	}

	return totalFit;
}

static inline unsigned int mostFit(unsigned int *fitness, unsigned int iCount){
	unsigned int fit = 0;
	for (unsigned int x = 1; x < iCount; x++){
		if (fitness[x] > fitness[fit]){
			fit = x;
		}
	}
	return fit;
}


void* geneticAlgorithm(unsigned int iCount, unsigned int gCount,
		getIndividual newRand, processInd getFitness, breeder breed,
		cloner clone, double breedRatio,
		mutator mutate, double mutateRatio){
	assert(UINT_MAX < ULONG_MAX / iCount + 1);

	void **pop = malloc(sizeof(void*) * iCount);
	unsigned int *fitness = malloc(sizeof(int) * iCount);

	void *bestIndv = NULL;
	unsigned int bestFit = 0;

	for (unsigned int x = 0; x < iCount; x++){
		pop[x] = newRand();
	}

	unsigned long totalFit;
	for (unsigned int g = 0; g < gCount; g++){
		totalFit = computeFitness(pop, fitness, getFitness, iCount);
		int tmp = mostFit(fitness, iCount);
		if (fitness[tmp] > bestFit){
			bestFit = fitness[tmp];
			if (bestIndv != NULL){
				free(bestIndv);
			}
			bestIndv = clone(pop[tmp]);
		}
		printf("Generation %03u: avg is %010u, best is %010u; "
			"overall best: %010u", g,
			(unsigned int) ((double) totalFit / iCount),
			fitness[tmp], bestFit);
		if (bestFit == fitness[tmp]){
			puts("^");
		}else{
			puts("");
		}

		reproduce(pop, iCount, clone, fitness, totalFit,
			breed, breedRatio, mutate, mutateRatio);
	}

	free(fitness);
	for(unsigned int x = 0; x<iCount; x++){
		free(pop[x]);
	}
	free(pop);
	return bestIndv;
}
