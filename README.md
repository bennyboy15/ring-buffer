# Atomic Ring Buffer in C

A simple implementation of both:

- Standard Ring Buffer
- Atomic Thread-Safe Ring Buffer

Written in C using atomics for safe concurrent access between threads.

---

# Features

## Standard Ring Buffer

- Ring buffer implementation
- Pointer-based read/write system
- Automatic wrap-around
- Overwrites oldest data when full

## Atomic Ring Buffer

- Lock-free implementation
- Uses `<stdatomic.h>`
- Prevents race conditions

---

# Project Structure

```text
.
├── main.c
├── main.h
└── README.md
```
---

# Ring Buffer Overview

A ring buffer is a fixed-size data structure that wraps back around to the start when overflowing at end.

Example:

```text
[ A ][ B ][ C ][ D ]
              ↑
            write

Next write wraps back to start.
```

---

# Standard Ring Buffer API

## Initialize

```c
RingBuff* rb = init();
```

Allocates and initializes a new ring buffer.

---

## Write

```c
write(rb, 'A');
```

Writes a character into the buffer.

If the buffer is full:

- Oldest data is overwritten
- Read pointer advances automatically

---

## Read

```c
char c = read(rb);
```

Reads one character from the buffer.

---

## Reset

```c
reset(rb);
```

Resets pointers and clears buffer contents.

---

## Free Memory

```c
freeRB(rb);
```

Frees allocated memory.

---

# Atomic Ring Buffer Overview

The atomic ring buffer is designed for safe concurrent access.

Uses:

- `atomic_load_explicit`
- `atomic_store_explicit`

---

# Atomic Ring Buffer API

## Initialize

```c
AtomicRingBuff* rb = initARB();
```

Creates a new atomic ring buffer.

---

## Write

```c
writeARB(rb, 'A');
```

Writes data safely using atomic operations.

If buffer is full:

- Write is ignored
- Existing data is preserved

---

## Read

```c
char c = readARB(rb);
```

Reads data safely using atomic operations.

---

## Reset

```c
resetARB(rb);
```

Resets head and tail atomically.

---

## Free Memory

```c
freeARB(rb);
```

Frees allocated memory.

---

# Notes

## Standard Ring Buffer
- Not thread-safe
- Best for single-threaded applications

## Atomic Ring Buffer
- Thread-safe
- Lock-free
- Ideal for:
  - Embedded systems
  - Networking
  - Audio pipelines
  - Producer/consumer systems

---
