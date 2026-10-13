
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    int src = open(argv[1], O_RDONLY);
    if (src == -1) {
        perror("Error opening source");
        return 1;
    }

    int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest == -1) {
        perror("Error opening destination");
        close(src);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int failed = 0;

    while ((bytes_read = read(src, buffer, sizeof(buffer))) > 0) {
        ssize_t total_written = 0;

        while (total_written < bytes_read) {
            ssize_t bytes_written = write(
                dest,
                buffer + total_written,
                bytes_read - total_written
            );

            if (bytes_written == -1) {
                if (errno == EINTR)
                    continue;

                perror("Error writing destination");
                failed = 1;
                break;
            }

            if (bytes_written == 0) {
                fprintf(stderr, "Write made no progress\n");
                failed = 1;
                break;
            }

            total_written += bytes_written;
        }

        if (failed)
            break;
    }

    if (bytes_read == -1) {
        perror("Error reading source");
        failed = 1;
    }

    if (close(src) == -1) {
        perror("Error closing source");
        failed = 1;
    }

    if (close(dest) == -1) {
        perror("Error closing destination");
        failed = 1;
    }

    if (failed)
        return 1;

    printf("File copied successfully: %s -> %s\n", argv[1], argv[2]);
    return 0;
}
