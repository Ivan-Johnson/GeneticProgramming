# GeneticProgramming

This repository implements a conventional algorithm and demonstrates its own
capabilities with sample programs of varying complexity. "test.c" is a minimal
demonstration of the genetic algorithm; individuals are strings of numeric
characters whose fitness is simply the value of the number they represent. The
executable "main-Ant" finds solutions to the artificial ant problem (as
described by John R. Koza in "Genetic Programming"), in which finite state
machines ("ants") are tasked with following a partially obfuscated trail of
"food" in a particular map (defined by MapGen/map.txt), such as this one:

     -----------------------------------------
    |                 X X                     |
    | X             X         X       X       |
    | X                       X             X |
    | X X X X                               X |
    |       X       X               X         |
    |       X       X           X X X         |
    |       X                                 |
    |       X       X                       X |
    |   X X X       X             X       X X |
    |   X                   X                 |
    |   X                                     |
    |   X X X                                 |
    |       X       X     X                   |
    |               X     X                   |
    |       X       X       X                 |
    |       X                             X X |
    | X   X X                               X |
    | X             X                       X |
    | X             X                       X |
    | X X   X     X       X X X X X X X X X X |
     -----------------------------------------

The only information that the ant receives is whether or not the tile directly
in front of it contains food. Based on its current state and its input, the ant
changes its state and takes an action (either turning nintey degrees in one
direction, or advancing a single tile and consuming the food there, if there is
any).

## Setup

Simply running `make` will compile the executables "test" and "main-Ant".

If `make` displays compilation errors, it is likely because your compiler
provides additional compilation warnings. Such errors can be suppressed by
changing the value of the variable "STRICT"; this can either be done my
modifying the makefile or by running `make STRICT=100` instead.

By default the makefile compiles in "debug" mode; this may be changed by
replacing the line `CFLAGS += $(OPTS_DEBUG)` in the makefile with `CFLAGS +=
$(OPTS_OPTIMIZED)`, doing so can decrease the runtimes by a factor of four or
more.

## Usage

If the preprocessor flag "DEBUG" was defined during compilation, the genetic
algorithm will print a message at the end of each generation. Otherwise, it will
have no output.

### main-Ant

"main-Ant" is run with no arguments.

Runtimes typically vary between two seconds and two minutes; most of the
variation is caused by the speed on your computer and whether the makefile was
configured to use debugging flags or optimization flags.

### test

"test" is run with no arguments. When complete, it will print the most fit
string it found to the console.

Runtimes typically vary between two seconds and two minutes; most of the
variation is caused by the speed on your computer and whether the makefile was
configured to use debugging flags or optimization flags.
