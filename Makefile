CFLAGS += -Wall -Wextra -Wfatal-errors -Werror -std=c99
#-lm

OPTS_DEBUG = -O0 -ggdb -fno-inline
OPTS_OPTIMIZED = -O3

CFLAGS += $(OPTS_DEBUG)
#CFLAGS += $(OPTS_OPTIMIZED)

all: main

main.exe: geneticAlgorithm.c helper.c main.c
	x86_64-w64-mingw32-gcc $(CFLAGS) -o main.exe main.c geneticAlgorithm.c helper.c

geneticAlgorithm.o: geneticAlgorithm.c geneticAlgorithm.h helper.h

main.o: main.c geneticAlgorithm.h

main: main.o geneticAlgorithm.o helper.o

clean:
	rm -f *.o main main.exe

.PHONY: all clean
