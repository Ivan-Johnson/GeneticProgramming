#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "helper.h"
#include "geneticAlgorithm.h"
#include "ant.h"
#include "world.h"

void* randAnt(){
	Ant *a = malloc(sizeof(Ant));
	a->state = 0;
	for(int x = 0; x<ANTINPUT_COUNT; x++) {
		for(int y = 0; y<STATE_COUNT; y++) {
			a->stateTable[x][y] = rand() % STATE_COUNT;
			a->actionTable[x][y] = rand() % ANTACTION_COUNT;
		}
	}
	return a;
}

World w;
static void initTheWorld(){
	w.x = 0;
	w.y = 0;
	w.facing = south;
	for(int x = 0; x<STATE_COUNT; x++) {
		for(int y = 0; y<ANTINPUT_COUNT; y++) {
			w.hasFood[x][y] = false;
		}
	}
	#include "MapGen/map.c"
}

unsigned int getFitness(void *ant){
	return WorldRun(w, ant, NULL);
}

void breedAnt(void *s1, void *s2){
	Ant *a1 = (Ant *) s1;
	Ant *a2 = (Ant *) s2;

	int i = 1 + rand() % (STATE_COUNT - 2);
	Ant tmp;

	for(int x = 0; x < ANTINPUT_COUNT; x++){
		memcpy(tmp.stateTable[x], a1->stateTable[x],
			sizeof(unsigned int) * i);
		memcpy(a1->stateTable[x], a2->stateTable[x],
			sizeof(unsigned int) * i);
		memcpy(a2->stateTable[x], tmp.stateTable[x],
			sizeof(unsigned int) * i);

		memcpy(tmp.actionTable[x], a1->actionTable[x],
			sizeof(AntActs) * i);
		memcpy(a1->actionTable[x], a2->actionTable[x],
			sizeof(AntActs) * i);
		memcpy(a2->actionTable[x], tmp.actionTable[x],
			sizeof(AntActs) * i);
	}
}

void mutateAnt(void *vp){
	Ant *a = (Ant*) vp;
	int iState = rand() % STATE_COUNT;
	int iInput = rand() % ANTINPUT_COUNT;
	if(rand() % 2){
		//randomize a single entry in the state table
		a->stateTable[iInput][iState] = rand() % STATE_COUNT;
	} else {
		//randomize a single entry in the action table
		a->actionTable[iInput][iState] = rand() % ANTACTION_COUNT;
	}
}

void* cloneAnt(void *asdf){
	Ant *a = (Ant*) asdf;
	Ant *b = AntClone(a);
	a->state = 0;
	b->state = 0;
	return b;
}

int main (int argc, char **argv){
	(void) argc;
	(void) argv;

	initTheWorld();
	int count = WorldCountFood(w);
	printf("Finite state machines are breed based on the number of pieces of \"food\" that\n"
		"they find. The maximal value for this world is %d.\n", count);

	srand(time(NULL));
	//srand(0);

	geneticParams p = geneticParamsDefault();
	p.popSize = 2000;
	p.genCount = 300;
	p.breedRatio = 0.5;
	p.mutateRatio = 0.1;

	Ant *a = geneticAlgorithm(p, randAnt, getFitness, cloneAnt,
				mutateAnt, breedAnt);

	int fitness = (signed) WorldRun(w, a, stdout);
	printf("Found %d of %d cells\n", fitness, count);
	free(a);
	return 0;
}
