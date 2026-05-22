#include "main.h"

RingBuff *init() {
    RingBuff *new_rb = malloc(sizeof(RingBuff)); // ALLOCATE DYNAMIC MEMORY FOR NEW RING BUFFER (DONT FORGET TO FREE THIS!!!)
    new_rb->read_ptr=&(new_rb->arr[0]);  // SET READ POINTER TO ADDRESS OF 1ST INDEX OF ARRAY
    new_rb->write_ptr=&(new_rb->arr[0]); // SET WRITE POINTER TO ADDRESS OF 1ST INDEX OF ARRAY
    memset(new_rb->arr, 0, BUFFER_SIZE); // INITIALISE ARRAY WITH ALL 0's (MAYBE A BETTER WAY TO DO THIS!?)
    return new_rb;
}

void reset(RingBuff *rb){
    rb->read_ptr = &(rb->arr[0]);
    rb->write_ptr = &(rb->arr[0]);
    rb->full = false;
    memset(rb->arr, 0, BUFFER_SIZE); // DO WE NEED TO DO THIS SINCE IT WILL OVERWRITE?
}

void freeRB(RingBuff *rb) {
    free(rb);
}

int spaceLeft(RingBuff *rb) {
    int space = 0;
    // CHECK ONLY IF BUFFER IS NOT FULL, SINCE IF IT IS FULL SPACE WILL BE 0 (DUH!)
    if (rb->full == false) {
        // IF WRITE POINTER IS AHEAD OF READ POINTER = BUFFER_SIZE - (WRITE_POINTER - READ_POINTER)
        if (rb->write_ptr >= rb->read_ptr) {
            space = BUFFER_SIZE - (rb->write_ptr - rb->read_ptr); // Ex: 5 - (4 - 1) = 2
        
        // IF READ POINTER IS AHEAD OF WRITE POINTER = (READ_POINTER - WRITE_POINTER)
        } else {
            space = (rb->read_ptr - rb->write_ptr);
        }
    }

    return space;
}

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
    RingBuff* rb = init();
    write(rb, 'G');
    write(rb, '\'');
    write(rb, 'D');
    write(rb, 'A');
    write(rb, 'Y');
    printf("%c", read(rb));
    return 0;
}