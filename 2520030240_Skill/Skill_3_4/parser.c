#include <stdio.h>
#include <string.h>

int main() {
    char input[200];

    printf("Enter command: ");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    /* Handle empty commands */
    if (strlen(input) == 0) {
        printf("Syntax Error: Empty command.\n");
        return 1;
    }

    /* Command cannot start with pipe */
    if (input[0] == '|') {
        printf("Syntax Error: Command cannot start with '|'.\n");
        return 1;
    }

    /* Command cannot end with pipe */
    int len = strlen(input);

    if (input[len - 1] == '|') {
        printf("Syntax Error: Command cannot end with '|'.\n");
        return 1;
    }

    /* Detect consecutive pipes */
    if (strstr(input, "||") != NULL) {
        printf("Syntax Error: Invalid pipe sequence.\n");
        return 1;
    }

    printf("\nParse successful.\n");
    printf("Execution structure:\n");

    /* Split command using pipe */
    char *command = strtok(input, "|");
    int number = 1;

    while (command != NULL) {
        printf("Command %d: %s\n", number, command);

        number++;
        command = strtok(NULL, "|");
    }

    return 0;
}
