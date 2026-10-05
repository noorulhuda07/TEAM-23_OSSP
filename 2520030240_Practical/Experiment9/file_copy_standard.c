#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    FILE *source, *destination;
    char buffer[4096];
    size_t bytesRead, bytesWritten;

    source = fopen("source.txt", "rb");

    if (source == NULL) {
        perror("Error opening source file");
        return 1;
    }

    destination = fopen("destination_standard.txt", "wb");

    if (destination == NULL) {
        perror("Error opening destination file");
        fclose(source);
        return 1;
    }

    clock_t start = clock();

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        bytesWritten = fwrite(buffer, 1, bytesRead, destination);

        if (bytesWritten != bytesRead) {
            perror("Error writing file");
            fclose(source);
            fclose(destination);
            return 1;
        }
    }

    clock_t end = clock();

    printf("File copied successfully using standard library I/O.\n");
    printf("CPU time: %.6f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    fclose(source);
    fclose(destination);

    return 0;
}

