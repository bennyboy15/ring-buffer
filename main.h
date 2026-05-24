#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define ATOMIC_BUFFER_SIZE 8

// RING BUFFER
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

// ATOMIC RING BUFFER
typedef struct {
  char arr[ATOMIC_BUFFER_SIZE];
  atomic_size_t head;
  atomic_size_t tail;
} AtomicRingBuff;

AtomicRingBuff *initARB();
void resetARB(AtomicRingBuff *rb);
void freeARB(AtomicRingBuff *rb);
int spaceLeftARB(AtomicRingBuff *rb);
void writeARB(AtomicRingBuff *rb, char newChar);
char readARB(AtomicRingBuff *rb);