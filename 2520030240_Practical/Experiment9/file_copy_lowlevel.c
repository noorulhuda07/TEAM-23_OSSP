#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int source, destination;
    char buffer[4096];
    ssize_t bytesRead, bytesWritten;
    off_t position;

    source = open("source.txt", O_RDONLY);

    if (source == -1) {
        perror("Error opening source file");
        return 1;
    }

    destination = open("destination_lowlevel.txt",
                       O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (destination == -1) {
        perror("Error opening destination file");
        close(source);
        return 1;
    }

    clock_t start = clock();

    while ((bytesRead = read(source, buffer, sizeof(buffer))) > 0) {
        bytesWritten = write(destination, buffer, bytesRead);

        if (bytesWritten != bytesRead) {
            perror("Error writing file");
            close(source);
            close(destination);
            return 1;
        }
    }

    position = lseek(source, 0, SEEK_CUR);

    clock_t end = clock();

    if (bytesRead == -1) {
        perror("Error reading file");
    }

    printf("File copied successfully using low-level I/O.\n");
    printf("Current file position: %ld bytes\n", (long)position);
    printf("CPU time: %.6f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    close(source);
    close(destination);

    return 0;
}


