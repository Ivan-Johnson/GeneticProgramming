#ifndef geneticAlgorithm_h
#define geneticAlgorithm_h

typedef void* (*getIndv)();
typedef unsigned int (*processIndv) (void*);
typedef void (*breeder) (void*, void *);
typedef void (*mutator) (void*);
typedef void* (*cloner) (void*);

struct geneticParams {
	unsigned int popSize;
	unsigned int genCount;
	double breedRatio;
	double mutateRatio;
};

typedef struct geneticParams geneticParams;

inline geneticParams geneticParamsDefault(){
	geneticParams p = {
		.popSize = 1024,
		.genCount = 128,
		.breedRatio = 0.5,
		.mutateRatio = 0.05,
	};
	return p;
}

void* geneticAlgorithm(geneticParams p, getIndv newRandom,
		processIndv getFitness, cloner, mutator, breeder);
#endif
