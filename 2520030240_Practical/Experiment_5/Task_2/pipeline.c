#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    int pipefd[2];
    pid_t ls_pid, grep_pid;

    /*
     * Create pipe
     * pipefd[0] = read end
     * pipefd[1] = write end
     */
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    /* Create first child for ls -l */
    ls_pid = fork();

    if (ls_pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (ls_pid == 0)
    {
        /* Child 1: ls -l */

        /* Redirect stdout to pipe */
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        execlp("ls", "ls", "-l", (char *)NULL);

        /* Only reached if exec fails */
        perror("execlp ls");
        exit(EXIT_FAILURE);
    }

    /* Create second child for grep ".c" */
    grep_pid = fork();

    if (grep_pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (grep_pid == 0)
    {
        /* Child 2: grep ".c" */

        /* Redirect stdin from pipe */
        if (dup2(pipefd[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(EXIT_FAILURE);
        }

        close(pipefd[0]);
        close(pipefd[1]);

        execlp("grep", "grep", ".c", (char *)NULL);

        /* Only reached if exec fails */
        perror("execlp grep");
        exit(EXIT_FAILURE);
    }

    /* Parent does not use the pipe */
    close(pipefd[0]);
    close(pipefd[1]);

    /* Wait for both children */
    waitpid(ls_pid, NULL, 0);
    waitpid(grep_pid, NULL, 0);

    printf("\nParent: Pipeline execution completed.\n");

    return 0;
}

