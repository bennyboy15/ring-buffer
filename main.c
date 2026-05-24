#include "main.h"

// RING BUFFER
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

// ATOMIC RING BUFFER
AtomicRingBuff *initARB(){
    AtomicRingBuff *newARB = calloc(1, sizeof(AtomicRingBuff));
    if (newARB == NULL) {
        return NULL;
    }
    atomic_init(&newARB->head, 0);
    atomic_init(&newARB->tail, 0);
    return newARB;
}
void resetARB(AtomicRingBuff *rb){
    if (rb == NULL) {
        return;
    }
    // atomic_store_explicit = sets value of atomic variable
    /*
        memory_order_seq_cst = Sequentially Consistent
        No Reordering: Do not move any memory operations that come before this line to after this line, and vice versa.
        Global Visibility: Process change to main memory immediately so every other thread sees it instantly.
        Total Order: All threads in the program will agree on the exact order in which this operation happened relative to other atomic operations.
    */
    atomic_store_explicit(&rb->head, 0, memory_order_seq_cst);
    atomic_store_explicit(&rb->tail, 0, memory_order_seq_cst);
}
void freeARB(AtomicRingBuff *rb){
    free(rb);
}
int spaceLeftARB(AtomicRingBuff *rb){
    // atomic_load_explicit = load value from atomic variable
    // Load the index head and tail is currently at to compare
    size_t head = atomic_load_explicit(&rb->head, memory_order_relaxed);
    size_t tail = atomic_load_explicit(&rb->tail, memory_order_relaxed);

    size_t used = (head - tail + ATOMIC_BUFFER_SIZE) % ATOMIC_BUFFER_SIZE;
    return (ATOMIC_BUFFER_SIZE - 1) - used;
}
void writeARB(AtomicRingBuff *rb, char newChar){
    size_t head = atomic_load_explicit(&rb->head, memory_order_relaxed);
    size_t tail = atomic_load_explicit(&rb->tail, memory_order_acquire);

    size_t next_head = (head + 1) % ATOMIC_BUFFER_SIZE; // Smarter way to get index to wrap back to start! Ex: 7+1 % 8 = 0 (BACK TO START!)

    // This checks if the next slot runs into the tail ("Waste One Slot" rule)
    if (next_head == tail) {
        return;
    }

    rb->arr[head] = newChar;
    // memory_order_release = tells other threads that you can use this slot in arr
    atomic_store_explicit(&rb->head, next_head, memory_order_release);
}
char readARB(AtomicRingBuff *rb){
    size_t head = atomic_load_explicit(&rb->head, memory_order_acquire);
    size_t tail = atomic_load_explicit(&rb->tail, memory_order_relaxed);

    if (tail == head) {
        return '\0';
    }
    size_t next_tail = (tail + 1) % BUFFER_SIZE;

    char read_value = rb->arr[tail];

    // memory_order_release = I am done reading this slot, other threads can overwrite this now!
    atomic_store_explicit(&rb->tail, next_tail, memory_order_release); 
    return read_value;
}

int main() {
    /*
    RingBuff* rb = init();
    write(rb, 'G');
    write(rb, '\'');
    write(rb, 'D');
    write(rb, 'A');
    write(rb, 'Y');
    printf("%c", read(rb));
    */
    return 0;
}