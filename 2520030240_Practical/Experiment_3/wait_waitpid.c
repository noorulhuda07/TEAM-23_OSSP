#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t child1, child2, child3;
    int status;

    printf("=== wait() and waitpid() Demonstration ===\n");
    printf("Parent PID: %d\n\n", getpid());

    /* Create first child */
    child1 = fork();

    if (child1 < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (child1 == 0)
    {
        printf("Child 1: PID = %d, PPID = %d\n", getpid(), getppid());
        sleep(2);
        printf("Child 1: Terminating\n");
        exit(10);
    }

    /* Create second child */
    child2 = fork();

    if (child2 < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (child2 == 0)
    {
        printf("Child 2: PID = %d, PPID = %d\n", getpid(), getppid());
        sleep(4);
        printf("Child 2: Terminating\n");
        exit(20);
    }

    /* Create third child */
    child3 = fork();

    if (child3 < 0)
    {
        perror("fork failed");
        return 1;
    }

    if (child3 == 0)
    {
        printf("Child 3: PID = %d, PPID = %d\n", getpid(), getppid());
        sleep(6);
        printf("Child 3: Terminating\n");
        exit(30);
    }

    /* Parent */
    printf("\nParent created three children:\n");
    printf("Child 1 PID: %d\n", child1);
    printf("Child 2 PID: %d\n", child2);
    printf("Child 3 PID: %d\n\n", child3);

    /* wait() waits for any child */
    printf("Parent using wait()...\n");

    pid_t finished = wait(&status);

    if (finished > 0)
    {
        if (WIFEXITED(status))
        {
            printf("wait(): Child PID %d terminated with exit status %d\n",
                   finished, WEXITSTATUS(status));
        }
    }

    /* waitpid() waits for a specific child */
    printf("\nParent using waitpid() for Child 3...\n");

    waitpid(child3, &status, 0);

    if (WIFEXITED(status))
    {
        printf("waitpid(): Child PID %d terminated with exit status %d\n",
               child3, WEXITSTATUS(status));
    }

    /* Collect remaining child */
    wait(NULL);

    printf("\nParent: All required children have been collected.\n");

    return 0;
}
