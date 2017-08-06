#ifndef geneticAlgorithm_h
#define geneticAlgorithm_h

typedef void* (*getIndividual)();
typedef unsigned int (*processInd) (void*);
typedef void (*breeder) (void*, void *);
typedef void (*mutator) (void*);
typedef void* (*cloner) (void*);

void* geneticAlgorithm(unsigned int iCount, unsigned int gCount,
		getIndividual getRand, processInd getFitness, breeder, cloner,
		double breedRatio, mutator, double mutateRatio);

#endif
