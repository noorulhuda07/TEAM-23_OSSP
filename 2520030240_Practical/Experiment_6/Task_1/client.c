#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SERVER_FIFO "/tmp/ossp_server_fifo"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int server_fd;
    char message[BUFFER_SIZE];

    /* Check command-line argument */
    if (argc < 2)
    {
        printf("Usage: %s \"message\"\n", argv[0]);
        return 1;
    }

    /* Get client PID */
    pid_t pid = getpid();

    /* Create unique response FIFO for this client */
    char response_fifo[256];

    snprintf(
        response_fifo,
        sizeof(response_fifo),
        "/tmp/ossp_client_%d_fifo",
        pid
    );

    unlink(response_fifo);

    if (mkfifo(response_fifo, 0666) == -1)
    {
        perror("mkfifo");
        return 1;
    }

    /* Open server FIFO */
    server_fd = open(SERVER_FIFO, O_WRONLY);

    if (server_fd == -1)
    {
        perror("open server FIFO");
        unlink(response_fifo);
        return 1;
    }

    /* Prepare request: PID|MESSAGE */
    snprintf(
        message,
        sizeof(message),
        "%d|%s",
        pid,
        argv[1]
    );

    /* Send message to server */
    write(server_fd, message, strlen(message));

    close(server_fd);

    printf("Client PID %d\n", pid);
    printf("Message sent: %s\n", argv[1]);

    /* Wait for server response */
    int response_fd = open(response_fifo, O_RDONLY);

    if (response_fd == -1)
    {
        perror("open response FIFO");
        unlink(response_fifo);
        return 1;
    }

    char response[BUFFER_SIZE];

    ssize_t bytes_read = read(
        response_fd,
        response,
        BUFFER_SIZE - 1
    );

    if (bytes_read > 0)
    {
        response[bytes_read] = '\0';

        printf("Server response: %s", response);
    }

    close(response_fd);

    /* Remove client's FIFO */
    unlink(response_fifo);

    return 0;
}

