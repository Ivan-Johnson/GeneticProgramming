#include "ant.h"
#include <stdio.h>
#include <stdlib.h>

Ant* AntClone(Ant *a){
	Ant *b = malloc(sizeof(Ant));
	b->state = a->state;
	for(int x = 0; x<ANTINPUT_COUNT; x++){
		for(int y = 0; y<STATE_COUNT; y++){
			b->stateTable[x][y] = a->stateTable[x][y];
			b->actionTable[x][y] = a->actionTable[x][y];
		}
	}
	return b;
}

AntActs AntAct(Ant *a, AntInput input){
	AntActs act = a->actionTable[input][a->state];
	a->state = a->stateTable[input][a->state];
	return act;
}
