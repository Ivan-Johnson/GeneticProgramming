#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "ant.h"
#include "world.h"

static inline void rotateLeft(Facing *f){
	(*f) = (*f + FACING_COUNT - 1) % FACING_COUNT;
}

static inline void rotateRight(Facing *f){
	(*f) = (*f + 1) % FACING_COUNT;
}

static inline void move(int *x, int *y, Facing f){
	switch (f){
	case north:
		(*y)--;
		break;
	case east:
		(*x)++;
		break;
	case south:
		(*y)++;
		break;
	case west:
		(*x)--;
		break;
	default:
		printf("Invalid facing");
		exit(EXIT_FAILURE);
	}
}

static inline bool isDead(int x, int y){
	return x < - ESCAPE_DIST || y < - ESCAPE_DIST ||
		x >= WORLD_WIDTH + ESCAPE_DIST ||
		y >= WORLD_WIDTH + ESCAPE_DIST;
}

static inline bool isOutOfBounds(int x, int y){
	return (x < 0 || y < 0 || x >= WORLD_WIDTH || y >= WORLD_WIDTH);
}

static inline bool isFacingFood(World *w){
	int x = w->x, y = w->y;
	move(&x, &y, w->facing);
	if (isOutOfBounds(x, y)) {
		return false;
	}
	return w->hasFood[x][y];
}

static inline char getFacingChar(Facing f){
	switch (f){
	case north:
		return '^';
	case east:
		return '>';
	case south:
		return 'V';
	case west:
		return '<';
	default:
		printf("Invalid facing");
		exit(EXIT_FAILURE);
	}
}

unsigned int WorldRun(World w, Ant *a, FILE *f){
	unsigned int numEaten = 0;
	int c = 0;
	while (c++ < MAX_STEP && !isDead(w.x, w.y)) {
		if (f){
			WorldPrint(&w, a, f);
		}
		AntActs aa = AntAct(a, isFacingFood(&w) ? food : nothing);
		switch (aa) {
		case turnLeft:
			rotateLeft(&w.facing);
			break;
		case moveForward:
			move(&(w.x), &(w.y), w.facing);
			if (! isOutOfBounds(w.x, w.y) && w.hasFood[w.x][w.y]) {
				numEaten++;
				w.hasFood[w.x][w.y] = false;
			}
			break;
		case turnRight:
			rotateRight(&w.facing);
			break;
		default:
			printf("Unknown ant action\n");
			exit (EXIT_FAILURE);
		}
	}
	if (f) {
		WorldPrint(&w, a, f);
	}
	return numEaten;
}

void WorldPrint(World *w, Ant *a, FILE *f){
	printf("x: %i, y: %i, face: %i, state: %i\n", w->x, w->y, w->facing,
		a->state);
	fprintf(f, " ");
	for(int y = 0; y<WORLD_WIDTH; y++) {
		fprintf(f, "__");
	}
	fprintf(f, "_\n");

	for(int y = 0; y<WORLD_WIDTH; y++) {
		fprintf(f, "| ");
		for(int x = 0; x<WORLD_WIDTH; x++) {
			if (x != w->x || y != w->y){
				fprintf(f, "%c ", w->hasFood[x][y] ? 'X' : ' ');
			} else {
				fprintf(f, "%c ", getFacingChar(w->facing));
			}
		}
		fprintf(f, "|\n");
	}
	fprintf(f, "|");
	for(int y = 0; y<WORLD_WIDTH; y++) {
		fprintf(f, "__");
	}
	fprintf(f, "_|\n");
}

int WorldCountFood(World w)
{
	int tot = 0;
	for (int a = 0; a < WORLD_WIDTH; a++) {
		for (int b = 0; b < WORLD_WIDTH; b++) {
			if (w.hasFood[a][b]) {
				if (tot == INT_MAX) {
					return -1;
				}
				tot++;
			}
		}
	}
	return tot;
}
