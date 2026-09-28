# OSSP 25CS2104E - Week 7 Task 1

## Title
Process Address Space and Memory Segment Analysis

## Aim

To write a C program that prints the addresses of code, global, static, heap, and stack variables, and to analyze the Linux process address space layout.

## Objectives

1. Identify different memory segments of a Linux process.
2. Print the addresses of variables belonging to different segments.
3. Understand the organization of virtual memory.
4. Examine `/proc/<PID>/maps`.
5. Relate program addresses to Linux virtual memory mappings.

## Theory

A Linux process has its own virtual address space. The operating system divides this address space into different regions for storing program instructions, variables, dynamically allocated memory, and function-call information.

### Major Memory Segments

| Segment | Purpose |
|---|---|
| Text/Code | Stores executable program instructions |
| Read-only Data | Stores read-only constants and program data |
| Data | Stores initialized global and static variables |
| BSS | Stores uninitialized global and static variables |
| Heap | Stores dynamically allocated memory |
| Stack | Stores local variables, function parameters, and function-call information |
| Shared Libraries | Contains libraries such as `libc` |
| Memory-mapped regions | Used for shared libraries, files, and other mappings |

## Program

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_variable = 100;
static int static_global_variable = 200;

void display_addresses(void)
{
    int local_variable = 300;
    static int static_local_variable = 400;

    int *heap_variable = malloc(sizeof(int));

    if (heap_variable == NULL)
    {
        perror("malloc failed");
        return;
    }

    *heap_variable = 500;

    printf("\n===== PROCESS MEMORY ADDRESS LAYOUT =====\n\n");

    printf("Code/Text address       : %p\n", (void *)display_addresses);
    printf("Global variable address : %p\n", (void *)&global_variable);
    printf("Static variable address : %p\n", (void *)&static_global_variable);
    printf("Static local address    : %p\n", (void *)&static_local_variable);
    printf("Heap variable address   : %p\n", (void *)heap_variable);
    printf("Stack variable address  : %p\n", (void *)&local_variable);

    printf("\nProcess ID (PID)        : %d\n", getpid());

    printf("\n===== MEMORY SEGMENTS =====\n");
    printf("Text/Code : Program instructions\n");
    printf("Data      : Initialized global/static variables\n");
    printf("BSS       : Uninitialized global/static variables\n");
    printf("Heap      : Dynamically allocated memory\n");
    printf("Stack     : Local variables and function calls\n");

    free(heap_variable);
}

int main(void)
{
    display_addresses();

    printf("\nProgram is running. PID = %d\n", getpid());
    printf("Press Enter to terminate...\n");

    getchar();

    return 0;
}
