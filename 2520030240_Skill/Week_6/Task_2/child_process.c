#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;

    printf("===== WEEK 6 - TASK 2 =====\n");
    printf("Child Process Execution\n\n");

    /* Create child process */
    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    /* Child process */
    if (pid == 0)
    {
        printf("Child Process:\n");
        printf("PID: %d\n", getpid());
        printf("Parent PID: %d\n", getppid());

        printf("Executing: ls -l\n\n");

        char *args[] = {"ls", "-l", NULL};

        if (execvp(args[0], args) == -1)
        {
            perror("execvp failed");
            exit(1);
        }
    }

    /* Parent process */
    else
    {
        int status;

        printf("Parent Process:\n");
        printf("PID: %d\n", getpid());
        printf("Created Child PID: %d\n\n", pid);

        /* Wait for child to finish */
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            return 1;
        }

        if (WIFEXITED(status))
        {
            printf("\nChild process exited with status: %d\n",
                   WEXITSTATUS(status));
        }
        else
        {
            printf("\nChild process did not exit normally.\n");
        }
    }

    return 0;
}
