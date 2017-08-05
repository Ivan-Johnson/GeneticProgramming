#ifndef geneticalgorithm_h
#define geneticalgorithm_h

typedef void* (*getInd)();
typedef int (*processInd) (void*);

void* geneticalgorithm(unsigned int iCount, unsigned int gCount, getInd getRand,
		processInd getFitness);

#endif
