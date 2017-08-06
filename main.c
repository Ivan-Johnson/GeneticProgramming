#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "helper.h"
#include "geneticAlgorithm.h"

int main (int argc, char **argv){
	(void) argc;
	(void) argv;

	for (int x = 0; x < 10; x++){
		printL(randl());
	}
	return 0;
}
