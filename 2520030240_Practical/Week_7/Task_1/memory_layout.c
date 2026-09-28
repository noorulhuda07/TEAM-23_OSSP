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

