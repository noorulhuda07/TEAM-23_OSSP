
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 4 || strlen(argv[3]) != 1) {
        fprintf(stderr, "Usage: %s <file> <offset> <character>\n",
                argv[0]);
        return 1;
    }

    char *end;
    long offset = strtol(argv[2], &end, 10);

    if (argv[2][0] == '\0' || *end != '\0' || offset < 0) {
        fprintf(stderr, "Invalid offset\n");
        return 1;
    }

    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    if (st.st_size == 0 || offset >= st.st_size) {
        fprintf(stderr, "File must be non-empty and offset in range\n");
        close(fd);
        return 1;
    }

    size_t length = (size_t)st.st_size;

    char *data = mmap(NULL, length, PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    printf("Original contents: %.*s", (int)length, data);

    data[offset] = argv[3][0];

    if (msync(data, length, MS_SYNC) == -1) {
        perror("msync");
        munmap(data, length);
        close(fd);
        return 1;
    }

    printf("\nUpdated contents:  %.*s", (int)length, data);

    int failed = 0;
    if (munmap(data, length) == -1) {
        perror("munmap");
        failed = 1;
    }

    if (close(fd) == -1) {
        perror("close");
        failed = 1;
    }

    return failed ? 1 : 0;
}
