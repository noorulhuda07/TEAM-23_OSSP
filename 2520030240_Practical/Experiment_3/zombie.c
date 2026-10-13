#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t pid;
    int status;

    printf("Parent PID: %d\n", getpid());
    fflush(stdout);

    pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (pid == 0)
    {
        printf("Child PID: %d\n", getpid());
        printf("Child is terminating...\n");
        exit(0);
    }
    else
    {
        printf("Parent created child PID: %d\n", pid);
        printf("Parent is waiting using wait()...\n");

        if (wait(&status) == -1)
        {
            perror("wait failed");
            return 1;
        }

        if (WIFEXITED(status))
        {
            printf("Child %d terminated with exit status %d\n",
                   pid, WEXITSTATUS(status));
        }

        printf("Child has been reaped. No zombie remains.\n");
    }

    return 0;
}
