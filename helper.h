#ifndef helper_h
#define helper_h

static const int longWidth = sizeof(long) * CHAR_BIT;
static const int intWidth = sizeof(int) * CHAR_BIT;

unsigned long randl();
void printL(unsigned long l);
void printI(unsigned int i);

#endif
