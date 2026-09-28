#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 100000

int main(void)
{
    int *data;
    pid_t pid;

    data = malloc(SIZE * sizeof(int));

    if (data == NULL)
    {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < SIZE; i++)
    {
        data[i] = i;
    }

    printf("===== WEEK 8 - TASK 2 =====\n");
    printf("Copy-on-Write Demonstration\n\n");

    printf("Parent PID : %d\n", getpid());
    printf("Data address in parent : %p\n", (void *)data);
    printf("Initial data[0] : %d\n\n", data[0]);

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        free(data);
        return 1;
    }

    if (pid == 0)
    {
        printf("----- CHILD PROCESS -----\n");
        printf("Child PID : %d\n", getpid());
        printf("Parent PID : %d\n", getppid());

        printf("Before modification:\n");
        printf("Child data[0] : %d\n", data[0]);
        printf("Child data address : %p\n", (void *)data);

        printf("\nChild is modifying data...\n");

        data[0] = 9999;

        printf("After modification:\n");
        printf("Child data[0] : %d\n", data[0]);
        printf("Child data address : %p\n", (void *)data);

        

printf("\nCopy-on-Write occurred when the child modified the page.\n");

printf("Child is paused. PID = %d\n", getpid());
printf("Press Enter to continue...\n");
getchar();

free(data);

        return 0;
    }
    else
    {
        printf("----- PARENT PROCESS -----\n");
        printf("Parent is waiting for child...\n");

        wait(NULL);

        printf("\nChild has completed.\n");
        printf("Parent data[0] : %d\n", data[0]);
        printf("Parent data address : %p\n", (void *)data);

        printf("\nParent data remains unchanged.\n");
        printf("This demonstrates process isolation after Copy-on-Write.\n");

        free(data);
    }

    return 0;
}

