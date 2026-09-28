#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t usr1_count = 0;
volatile sig_atomic_t terminate_flag = 0;

/*
 * Signal handler for SIGINT, SIGTERM and SIGUSR1
 */
void signal_handler(int signal)
{
    const char *message;

    switch (signal)
    {
        case SIGINT:
            message = "\n[HANDLER] SIGINT received - Interrupt signal.\n";
            write(STDOUT_FILENO, message, strlen(message));
            terminate_flag = 1;
            break;

        case SIGTERM:
            message = "\n[HANDLER] SIGTERM received - Termination signal.\n";
            write(STDOUT_FILENO, message, strlen(message));
            terminate_flag = 1;
            break;

        case SIGUSR1:
            message = "\n[HANDLER] SIGUSR1 received - User-defined signal.\n";
            write(STDOUT_FILENO, message, strlen(message));
            usr1_count++;
            break;

        default:
            break;
    }
}

int main(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    /* Register SIGINT handler */
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction SIGINT");
        return 1;
    }

    /* Register SIGTERM handler */
    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        perror("sigaction SIGTERM");
        return 1;
    }

    /* Register SIGUSR1 handler */
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction SIGUSR1");
        return 1;
    }

    printf("========================================\n");
    printf("       POSIX SIGNAL HANDLING\n");
    printf("========================================\n");
    printf("Process PID: %d\n", getpid());
    printf("Waiting for signals...\n\n");

    printf("Send SIGUSR1 : kill -USR1 %d\n", getpid());
    printf("Send SIGTERM : kill -TERM %d\n", getpid());
    printf("Send SIGINT  : kill -INT %d\n", getpid());

    while (!terminate_flag)
    {
        pause();

        if (usr1_count > 0)
        {
            printf("[MAIN] SIGUSR1 count: %d\n", usr1_count);
            usr1_count = 0;
        }
    }

    printf("\n[MAIN] Termination flag detected.\n");
    printf("[MAIN] Program exiting gracefully.\n");

    return 0;
}

