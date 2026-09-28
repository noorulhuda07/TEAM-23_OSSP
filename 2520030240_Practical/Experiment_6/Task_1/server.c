#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#define SERVER_FIFO "/tmp/ossp_server_fifo"
#define BUFFER_SIZE 1024

int main(void)
{
    int server_fd;
    char buffer[BUFFER_SIZE];

    /* Remove old FIFO if it exists */
    unlink(SERVER_FIFO);

    /* Create the named pipe */
    if (mkfifo(SERVER_FIFO, 0666) == -1)
    {
        perror("mkfifo");
        return 1;
    }

    printf("========================================\n");
    printf("      OSSP FIFO SERVER\n");
    printf("========================================\n");
    printf("Server FIFO: %s\n", SERVER_FIFO);
    printf("Waiting for clients...\n\n");

    /* Open server FIFO for reading */
    server_fd = open(SERVER_FIFO, O_RDONLY);

    if (server_fd == -1)
    {
        perror("open");
        unlink(SERVER_FIFO);
        return 1;
    }

    while (1)
    {
        ssize_t bytes_read = read(server_fd, buffer, BUFFER_SIZE - 1);

        if (bytes_read > 0)
        {
            buffer[bytes_read] = '\0';

            /* Request format: PID|MESSAGE */
            char *separator = strchr(buffer, '|');

            if (separator == NULL)
            {
                printf("Invalid request received: %s\n", buffer);
                continue;
            }

            *separator = '\0';

            char *client_pid = buffer;
            char *message = separator + 1;

            printf("----------------------------------------\n");
            printf("Client PID : %s\n", client_pid);
            printf("Message    : %s\n", message);

            /* Create client's response FIFO path */
            char response_fifo[256];

            snprintf(
                response_fifo,
                sizeof(response_fifo),
                "/tmp/ossp_client_%s_fifo",
                client_pid
            );

            /* Prepare response */
            char response[BUFFER_SIZE];

            snprintf(
                response,
                sizeof(response),
                "Server processed message from Client PID %s: \"%s\"\n",
                client_pid,
                message
            );

            /* Open client's response FIFO */
            int response_fd = open(response_fifo, O_WRONLY);

            if (response_fd == -1)
            {
                perror("open response FIFO");
                continue;
            }

            /* Send response */
            write(response_fd, response, strlen(response));

            close(response_fd);

            printf("Response sent to Client PID %s\n", client_pid);
        }
        else if (bytes_read == -1)
        {
            perror("read");
            break;
        }
    }

    close(server_fd);
    unlink(SERVER_FIFO);

    return 0;
}
