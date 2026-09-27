#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int global_variable = 100;
static int static_variable = 200;

int main(void)
{
    int stack_variable = 300;

    int *heap_variable = malloc(sizeof(int));

    if (heap_variable == NULL)
    {
        perror("malloc failed");
        return 1;
    }

    *heap_variable = 400;

    printf("===== WEEK 7 - TASK 2 =====\n");
    printf("Virtual Memory Mapping Analysis\n\n");

    printf("Process PID       : %d\n", getpid());
    printf("Global address    : %p\n", (void *)&global_variable);
    printf("Static address    : %p\n", (void *)&static_variable);
    printf("Heap address      : %p\n", (void *)heap_variable);
    printf("Stack address     : %p\n", (void *)&stack_variable);

    printf("\nProcess is running.\n");
    printf("Use another terminal to inspect /proc/%d/maps\n", getpid());
    printf("Press Enter to terminate...\n");

    getchar();

    free(heap_variable);

    return 0;
}
