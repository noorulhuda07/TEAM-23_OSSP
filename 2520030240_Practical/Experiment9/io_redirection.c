#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int file;

    file = open("redirected_output.txt",
                O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (file == -1) {
        perror("Error opening file");
        return 1;
    }

    if (dup2(file, STDOUT_FILENO) == -1) {
        perror("dup2 failed");
        close(file);
        return 1;
    }

    close(file);

    printf("This output has been redirected to a file.\n");
    printf("dup2() makes the file descriptor act as standard output.\n");

    return 0;
}

