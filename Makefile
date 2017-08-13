#to cross-compile for windows, uncomment. Executables must be renamed to .exe
#CC = x86_64-w64-mingw32-gcc

CFLAGS += -Wall -Wextra -Wfatal-errors -Werror -std=c99
LDLIBS = -lm



OPTS_DEBUG = -D DEBUG -O0 -ggdb -fno-inline
OPTS_OPTIMIZED = -O3

CFLAGS += $(OPTS_DEBUG)
#CFLAGS += $(OPTS_OPTIMIZED)



all: test

ant.o: ant.h

geneticAlgorithm.o: geneticAlgorithm.h helper.h

test.o: geneticAlgorithm.h helper.h

test: test.o geneticAlgorithm.o helper.o

world.o: world.h ant.h

MapGen/map.c: MapGen/map.txt MapGen/convertMap.bash
	./MapGen/convertMap.bash $< > $@
clean:
	rm -f *.o test

.PHONY: all clean all_exe
