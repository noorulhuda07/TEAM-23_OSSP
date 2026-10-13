#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status;

    printf("Parent process PID: %d\n", getpid());
    printf("Creating child process using fork()...\n");
    fflush(stdout);

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        printf("\nChild process PID: %d\n", getpid());
        printf("Child's parent PID: %d\n", getppid());
        printf("Executing command: uname -s using exec()\n");
        fflush(stdout);

        execlp("uname", "uname", "-s", (char *)NULL);

        /* Reached only if exec fails */
        perror("exec failed");
        _exit(127);
    }
    else
    {
        printf("\nParent is waiting for child PID: %d\n", pid);

        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            return 1;
        }

        if (WIFEXITED(status))
        {
            printf("\nChild exited with status: %d\n",
                   WEXITSTATUS(status));
        }
        else
        {
            printf("\nChild did not exit normally.\n");
        }

        printf("Parent PID %d: child execution completed.\n", getpid());
    }

    return 0;
}
