#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void) {
    int fd = open("input.txt", O_RDONLY);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    if (lseek(fd, 7, SEEK_SET) == (off_t)-1) {
        perror("lseek");
        close(fd);
        return 1;
    }

    char buffer[100];
    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);

    if (n == -1) {
        perror("read");
        close(fd);
        return 1;
    }

    buffer[n] = '\0';
    printf("Content from offset 7: %s\n", buffer);

    if (close(fd) == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
