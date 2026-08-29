#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX_HISTORY 10
#define MAX_INPUT 100

char *history[MAX_HISTORY];
int history_count = 0;

void add_history(const char *command) {
    if (strlen(command) == 0)
        return;

    if (history_count < MAX_HISTORY) {
        history[history_count] = malloc(strlen(command) + 1);
        strcpy(history[history_count], command);
        history_count++;
    } else {
        free(history[0]);

        for (int i = 1; i < MAX_HISTORY; i++)
            history[i - 1] = history[i];

        history[MAX_HISTORY - 1] = malloc(strlen(command) + 1);
        strcpy(history[MAX_HISTORY - 1], command);
    }
}

void free_history() {
    for (int i = 0; i < history_count; i++)
        free(history[i]);
}

int main() {
    char input[MAX_INPUT];
    int history_index = history_count;

    printf("Interactive Command History\n");
    printf("Use UP/DOWN arrows to navigate history.\n");
    printf("Type 'exit' to quit.\n\n");

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        struct termios oldt, newt;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        int pos = 0;
        char ch;

        while (1) {
            ch = getchar();

            if (ch == '\n') {
                input[pos] = '\0';
                printf("\n");
                break;
            }

            if (ch == 27) {
                char ch1 = getchar();
                char ch2 = getchar();

                if (ch1 == '[' && ch2 == 'A') {
                    if (history_count > 0 && history_index > 0) {
                        history_index--;
                        strcpy(input, history[history_index]);
                        pos = strlen(input);

                        printf("\r\033[Kmyshell> %s", input);
                        fflush(stdout);
                    }
                }

                else if (ch1 == '[' && ch2 == 'B') {
                    if (history_index < history_count - 1) {
                        history_index++;
                        strcpy(input, history[history_index]);
                        pos = strlen(input);

                        printf("\r\033[Kmyshell> %s", input);
                        fflush(stdout);
                    } else {
                        history_index = history_count;
                        input[0] = '\0';
                        pos = 0;

                        printf("\r\033[Kmyshell> ");
                        fflush(stdout);
                    }
                }
            }

            else if (ch == 127) {
                if (pos > 0) {
                    pos--;
                    input[pos] = '\0';

                    printf("\b \b");
                    fflush(stdout);
                }
            }

            else if (pos < MAX_INPUT - 1) {
                input[pos++] = ch;
                putchar(ch);
                fflush(stdout);
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

        if (strcmp(input, "exit") == 0)
            break;

        if (strlen(input) == 0)
            continue;

        add_history(input);
        history_index = history_count;

        printf("Command stored: %s\n", input);
    }

    free_history();

    printf("Exiting shell...\n");

    return 0;
}
