#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int status;

    printf("===== WEEK 7 - TASK 1 =====\n");
    printf("Process Synchronization and Child Monitoring\n\n");

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child Process Started\n");
        printf("Child PID: %d\n", getpid());
        printf("Parent PID: %d\n\n", getppid());

        printf("Child is performing work...\n");

        sleep(3);

        printf("Child work completed.\n");
        printf("Child exiting with status 5.\n");

        exit(5);
    }
    else
    {
        printf("Parent Process Started\n");
        printf("Parent PID: %d\n", getpid());
        printf("Monitoring Child PID: %d\n\n", pid);

        printf("Parent is waiting for the child using waitpid()...\n");

        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            return 1;
        }

        printf("\nChild process has terminated.\n");

        if (WIFEXITED(status))
        {
            printf("Child exited normally.\n");
            printf("Child exit status: %d\n", WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("Child was terminated by a signal.\n");
            printf("Signal number: %d\n", WTERMSIG(status));
        }
    }

    return 0;
}

