#include "main.h"

void write(RingBuff *rb, char newChar) {

    // IF BUFFER IS FULL
    if (rb->full == true) {
        rb->read_ptr += 1; // MOVE READ POINTER
        // IF READ POINTER AT END OF BUFFER, GO BACK TO START (RING!!)
        if (rb->read_ptr > &(rb->arr[BUFFER_SIZE-1])) {
            rb->read_ptr = &(rb->arr[0]);
        }
    }

    *(rb->write_ptr) = newChar; // WRITE TO BUFFER
    rb->write_ptr += 1; // MOVE WRITE POINTER
    // IF WRITE POINTER AT END OF BUFFER, GO BACK TO START (RING!!)
    if (rb->write_ptr > &(rb->arr[BUFFER_SIZE-1])) {
        rb->write_ptr = &(rb->arr[0]);
    }
    // IF READ POINTER CATCHES UP WITH WRITE POINTER, BUFFER MUST BE FULL
    if (rb->read_ptr == rb->write_ptr) {
        rb->full = true;
    }
}

char read(RingBuff *rb) {
    char data;
    if (rb->write_ptr != rb->read_ptr || rb->full) {
        data = *(rb->read_ptr); // GET VALUE @ READ POINTER BY DEREFENCING
        rb->read_ptr += 1;
        rb->full = false;

        // IF READ POINTER AT END OF BUFFER, GO BACK TO START (RING!!)
        if (rb->read_ptr > &(rb->arr[BUFFER_SIZE-1])) {
            rb->read_ptr = &(rb->arr[0]);
        }
    }
    return data;
}

int main() {
    RingBuff ringBuffer = {.arr = {'a', 'b', 'c', 'd', 'e'}};
    printf("%c", ringBuffer.arr[0]);
}