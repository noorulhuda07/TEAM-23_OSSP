#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void display_process_info(const char *stage)
{
    char path[100];
    char state;
    int pid;
    char comm[100];

    snprintf(path, sizeof(path), "/proc/%d/stat", getpid());

    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        perror("Unable to open /proc");
        return;
    }

    fscanf(file, "%d %s %c", &pid, comm, &state);
    fclose(file);

    printf("[%s]\n", stage);
    printf("PID  : %d\n", getpid());
    printf("PPID : %d\n", getppid());
    printf("State: %c\n\n", state);
}

int main()
{
    pid_t pid;

    printf("=== Process State Demonstration ===\n\n");

    /* Parent process */
    display_process_info("Parent - Running");

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    /* Child process */
    if (pid == 0)
    {
        printf("Child process created.\n");

        display_process_info("Child - Running");

        printf("Child entering waiting/sleeping state for 60 seconds...\n");
        sleep(60);

        display_process_info("Child - Running Again");
        printf("Child terminating...\n");

        exit(0);
    }

    /* Parent process */
    else
    {
        printf("Parent process created child.\n");
        printf("Parent PID : %d\n", getpid());
        printf("Child PID  : %d\n\n", pid);

        printf("Parent waiting for child...\n");

        display_process_info("Parent - Waiting");

        wait(NULL);

        printf("\nChild has terminated.\n");

        display_process_info("Parent - Running After wait");
    }

    return 0;
}
