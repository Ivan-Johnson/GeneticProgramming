#include <limits.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "geneticAlgorithm.h"
#include "helper.h"

extern geneticParams geneticParamsDefault();

static const double SKEW_FACTOR = 0.1;

static int getRandomWeighted(unsigned int *fitness, int popSize,
			unsigned long totalFit){
	unsigned long randomFit;
	if ((unsigned long) popSize < (unsigned long) (SKEW_FACTOR * ULONG_MAX)){
		randomFit = randl() % totalFit;
	} else {
		do {
			randomFit = randl();
		} while (randomFit >= totalFit);
	}

	int i = 0;

	while (randomFit >= fitness[i]){
		assert(i < popSize);
		randomFit -= fitness[i];
		i++;
	}

	return i;
}

static void reproduce(void **pop, int popSize, cloner clone,
		unsigned int *fitness, unsigned long totalFit,
		breeder breed, double breedRatio,
		mutator mutate, double mutateRatio){
	if (breed == NULL){
		breedRatio = 0;
	}
	if (mutate == NULL){
		mutateRatio = 0;
	}
	int breedCount = (int) (popSize * breedRatio);
	int mutateCount = (int) (popSize * mutateRatio);
	assert(breedCount + mutateCount <= popSize);

	void **popWorking = malloc(sizeof(void*) * popSize);

	//create the next generation
	for (int x = 0; x < popSize; x++){
		int i = getRandomWeighted(fitness, popSize, totalFit);
		popWorking[x] = clone(pop[i]);
	}

	//breed the next generation
	if (breedCount % 2){//breed the odd child, if any
		breedCount--;
		unsigned tmpI = getRandomWeighted(fitness, popSize, totalFit);
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
	for (int x = 0; x < popSize; x++){
		free(pop[x]);
		pop[x] = popWorking[x];
	}
	free(popWorking);
}

static unsigned long computeFitness(void **pop, unsigned int *fitness,
				processIndv getFitness, unsigned int popSize){
	unsigned long totalFit = 0;
	for (unsigned int x = 0; x < popSize; x++){
		fitness[x] = getFitness(pop[x]);
		totalFit += fitness[x];
	}

	return totalFit;
}

static inline unsigned int mostFit(unsigned int *fitness, unsigned int popSize){
	unsigned int fit = 0;
	for (unsigned int x = 1; x < popSize; x++){
		if (fitness[x] > fitness[fit]){
			fit = x;
		}
	}
	return fit;
}

void* geneticAlgorithm(geneticParams p, getIndv newRand,
		processIndv getFitness, cloner clone, mutator mutate,
		breeder breed){
	void **pop = malloc(sizeof(void*) * p.popSize);
	unsigned int *fitness = malloc(sizeof(int) * p.popSize);

	void *bestIndv = NULL;
	unsigned int bestFit = 0;

	for (unsigned int x = 0; x < p.popSize; x++){
		pop[x] = newRand();
	}

	unsigned long totalFit;
	for (unsigned int g = 0; g < p.genCount; g++){
		totalFit = computeFitness(pop, fitness, getFitness, p.popSize);
		int tmp = mostFit(fitness, p.popSize);
		if (fitness[tmp] > bestFit){
			bestFit = fitness[tmp];
			if (bestIndv != NULL){
				free(bestIndv);
			}
			bestIndv = clone(pop[tmp]);
		}
		printf("Generation %03u: avg is %010u, best is %010u; "
			"overall best: %010u", g,
			(unsigned int) ((double) totalFit / p.popSize),
			fitness[tmp], bestFit);
		if (bestFit == fitness[tmp]){
			puts("^");
		}else{
			puts("");
		}

		reproduce(pop, p.popSize, clone, fitness, totalFit,
			breed, p.breedRatio, mutate, p.mutateRatio);
	}

	free(fitness);
	for(unsigned int x = 0; x<p.popSize; x++){
		free(pop[x]);
	}
	free(pop);
	return bestIndv;
}
