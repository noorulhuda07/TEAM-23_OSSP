
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* Redirect standard input from input.txt */
        int in_fd = open("input.txt", O_RDONLY);
        if (in_fd == -1) {
            perror("open input.txt");
            _exit(1);
        }

        if (dup2(in_fd, STDIN_FILENO) == -1) {
            perror("dup2 stdin");
            close(in_fd);
            _exit(1);
        }
        close(in_fd);

        /* Redirect standard output to redirected_output.txt */
        int out_fd = open("redirected_output.txt",
                          O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd == -1) {
            perror("open redirected_output.txt");
            _exit(1);
        }

        if (dup2(out_fd, STDOUT_FILENO) == -1) {
            perror("dup2 stdout");
            close(out_fd);
            _exit(1);
        }
        close(out_fd);

        /* Behaves like a command reading stdin and writing stdout */
        char buffer[256];
        ssize_t n;

        while ((n = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0) {
            ssize_t written = 0;

            while (written < n) {
                ssize_t w = write(STDOUT_FILENO,
                                  buffer + written,
                                  (size_t)(n - written));
                if (w < 0) {
                    perror("write");
                    _exit(1);
                }
                if (w == 0) {
                    _exit(1);
                }
                written += w;
            }
        }

        if (n < 0) {
            perror("read");
            _exit(1);
        }

        _exit(0);
    }

    int status;
    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        return 1;
    }

    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        fprintf(stderr, "Redirection child failed\n");
        return 1;
    }

    printf("Redirection completed. Check redirected_output.txt\n");
    return 0;
}
