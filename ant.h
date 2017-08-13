#ifndef ant_h
#define ant_h

#include <stdbool.h>

#define STATE_COUNT 10

#define ANTINPUT_COUNT 2
enum AntInput{
	//must count up from zero, as this is used as an array index
	nothing=0, food
};
typedef enum AntInput AntInput;

#define ANTACTION_COUNT 3
enum AntActs{
	turnLeft=0, moveForward, turnRight,
};
typedef enum AntActs AntActs;

struct Ant {
	unsigned int state;
	unsigned int stateTable[ANTINPUT_COUNT][STATE_COUNT];
	AntActs actionTable[ANTINPUT_COUNT][STATE_COUNT];
};
typedef struct Ant Ant;

Ant* AntClone(Ant*);
AntActs AntAct(Ant*, AntInput);

#endif
