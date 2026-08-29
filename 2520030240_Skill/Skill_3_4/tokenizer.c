#include <stdio.h>
#include <string.h>

#define MAX_TOKENS 20

int main() {
    char input[200];
    char *tokens[MAX_TOKENS];
    int count = 0;

    printf("Enter command: ");
    fgets(input, sizeof(input), stdin);

    input[strcspn(input, "\n")] = '\0';

    char *token = strtok(input, " \t");

    while (token != NULL && count < MAX_TOKENS) {
        tokens[count] = token;
        count++;
        token = strtok(NULL, " \t");
    }

    printf("\nTokens:\n");

    for (int i = 0; i < count; i++) {
        printf("Token %d: %s\n", i + 1, tokens[i]);
    }

    if (count == 0) {
        printf("Empty command.\n");
    }

    return 0;
}
