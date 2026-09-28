# OSSP 25CS2104E - Week 8 Task 1

## Title

Dynamic Memory Allocation and Memory Leak Detection Using Valgrind

## Aim

To develop a C program demonstrating `malloc()`, `calloc()`, `realloc()`, and `free()` and analyze dynamic memory allocation and memory leaks using Valgrind.

## Objectives

1. Demonstrate dynamic memory allocation using `malloc()`.
2. Demonstrate zero-initialized allocation using `calloc()`.
3. Resize allocated memory using `realloc()`.
4. Release dynamically allocated memory using `free()`.
5. Detect memory leaks using Valgrind.

## Theory

Dynamic memory allocation allows a program to request memory during runtime from the heap.

### malloc()

`malloc()` allocates a specified number of bytes of uninitialized memory.

```c
int *ptr = malloc(5 * sizeof(int));
