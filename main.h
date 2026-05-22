#include <stdlib.h>
#include <stdbool.h>

const int BUFFER_SIZE = 5;
typedef struct RingBuff {
    char *read_ptr;
    char *write_ptr;
    char arr[BUFFER_SIZE];
    bool full;
} RingBuff;

RingBuff *init();
void reset(RingBuff *rb);
void freeRB(RingBuff *rb);
int spaceLeft(RingBuff *rb);
void write(RingBuff *rb, char newChar);
char read(RingBuff *rb);