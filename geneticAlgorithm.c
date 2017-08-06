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
	/* //does not work because randomFit is unsigned
	do {
		randomFit -= fitness[i++];
	} while (randomFit >= 0);
	*/
	while (randomFit >= fitness[i]){
		assert(i < iCount);
		randomFit -= fitness[i];
		i++;
	}

	return i;
}

static void breed(void **pop, unsigned int *fitness, int iCount,
		unsigned long totalFit, double breedRatio, breeder b, cloner c){
	int breedCount = (int) (iCount * breedRatio + 1);
	void **popWorking = malloc(sizeof(void*) * iCount);

	int x;
	for (x = 0; x < breedCount; x += 2){
		int p1, p2;
		p1 = getRandomWeighted(fitness, iCount, totalFit);
		p2 = getRandomWeighted(fitness, iCount, totalFit);
		popWorking[x] = c(pop[p1]);
		popWorking[x+1] = c(pop[p2]);
		b(popWorking[x], popWorking[x+1]);
	}

	for (/*x ~= breedCount*/; x < iCount; x++){
		int i = getRandomWeighted(fitness, iCount, totalFit);
		popWorking[x] = c(pop[i]);
	}

	for (int x = 0; x < iCount; x++){
		free(pop[x]);
		pop[x] = popWorking[x];
	}
	free(popWorking);
}

static void mutate(mutator m, double mutateRatio){
	(void) m;
	(void) mutateRatio;
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
		getIndividual newRand, processInd getFitness, breeder b,
		cloner c, double breedRatio, mutator m, double mutateRatio){
	//ensure that total fitness will never overflow
	//apparently gcc thinks it's impossible though
	//assert(UINT_MAX * iCount < ULONG_MAX);

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
		printf("Generation %u average fitness is %u\n", g,
			(unsigned int) ((double) totalFit / iCount));
		int tmp = mostFit(fitness, iCount);
		if (fitness[tmp] > bestFit){
			bestFit = fitness[tmp];
			if (bestIndv != NULL){
				free(bestIndv);
			}
			bestIndv = c(pop[tmp]);
		}

		if (b != NULL && breedRatio != 0){
			breed(pop, fitness, iCount, totalFit, breedRatio, b, c);
		}
		if (m != NULL && mutateRatio != 0){
			mutate(m, mutateRatio);
		}
	}

	free(fitness);
	for(unsigned int x = 0; x<iCount; x++){
		free(pop[x]);
	}
	free(pop);
	return bestIndv;
}
