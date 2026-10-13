#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int source_fd, dest_fd;
    char buffer[4096];
    ssize_t bytes_read, bytes_written;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <source> <destination>\n", argv[0]);
        return 1;
    }

    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1)
    {
        perror("open source");
        return 1;
    }

    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1)
    {
        perror("open destination");
        close(source_fd);
        return 1;
    }

    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0)
    {
        ssize_t total_written = 0;

        while (total_written < bytes_read)
        {
            bytes_written = write(dest_fd, buffer + total_written,
                                  (size_t)(bytes_read - total_written));

            if (bytes_written == -1)
            {
                if (errno == EINTR)
                    continue;

                perror("write");
                close(source_fd);
                close(dest_fd);
                return 1;
            }

            if (bytes_written == 0)
            {
                fprintf(stderr, "write returned zero\n");
                close(source_fd);
                close(dest_fd);
                return 1;
            }

            total_written += bytes_written;
        }
    }

    if (bytes_read == -1)
    {
        perror("read");
        close(source_fd);
        close(dest_fd);
        return 1;
    }

    if (close(source_fd) == -1)
        perror("close source");

    if (close(dest_fd) == -1)
        perror("close destination");

    printf("Copied '%s' to '%s' successfully.\n", argv[1], argv[2]);
    return 0;
}
