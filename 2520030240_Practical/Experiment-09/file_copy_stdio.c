
#include <stdio.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    FILE *src = fopen(argv[1], "rb");
    if (src == NULL) {
        perror("Error opening source");
        return 1;
    }

    FILE *dest = fopen(argv[2], "wb");
    if (dest == NULL) {
        perror("Error opening destination");
        fclose(src);
        return 1;
    }

    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    int failed = 0;

    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        size_t total_written = 0;

        while (total_written < bytes_read) {
            size_t n = fwrite(
                buffer + total_written,
                1,
                bytes_read - total_written,
                dest
            );

            if (n == 0) {
                perror("Error writing destination");
                failed = 1;
                break;
            }

            total_written += n;
        }

        if (failed)
            break;
    }

    if (ferror(src)) {
        perror("Error reading source");
        failed = 1;
    }

    if (fclose(src) == EOF) {
        perror("Error closing source");
        failed = 1;
    }

    if (fclose(dest) == EOF) {
        perror("Error closing destination");
        failed = 1;
    }

    if (failed)
        return 1;

    printf("File copied successfully using standard I/O: %s -> %s\n",
           argv[1], argv[2]);

    return 0;
}
