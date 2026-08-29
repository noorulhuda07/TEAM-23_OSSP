#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 20

typedef struct Node {
    char *command;
    struct Node *next;
} Node;

/* Add a command to the linked list */
void add_command(Node **head, const char *command) {
    Node *new_node = malloc(sizeof(Node));

    if (new_node == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    new_node->command = malloc(strlen(command) + 1);

    if (new_node->command == NULL) {
        free(new_node);
        printf("Memory allocation failed.\n");
        return;
    }

    strcpy(new_node->command, command);

    new_node->next = *head;
    *head = new_node;
}

/* Display all commands */
void display_commands(Node *head) {
    printf("\nStored Commands:\n");

    while (head != NULL) {
        printf("- %s\n", head->command);
        head = head->next;
    }
}

/* Free the entire linked list */
void free_commands(Node *head) {
    Node *temp;

    while (head != NULL) {
        temp = head;
        head = head->next;

        free(temp->command);
        free(temp);
    }
}

int main() {
    char *buffer;
    size_t buffer_size = INITIAL_SIZE;
    Node *head = NULL;

    /* Dynamic buffer allocation */
    buffer = malloc(buffer_size);

    if (buffer == NULL) {
        printf("Initial memory allocation failed.\n");
        return 1;
    }

    printf("Dynamic Memory and Linked List Demo\n");
    printf("Enter commands. Type 'done' to finish.\n\n");

    while (1) {
        printf("Enter command: ");
        fflush(stdout);

        if (fgets(buffer, buffer_size, stdin) == NULL)
            break;

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "done") == 0)
            break;

        if (strlen(buffer) == 0)
            continue;

        /* Resize buffer if input fills the buffer */
        if (strlen(buffer) >= buffer_size - 1) {
            buffer_size *= 2;

            char *new_buffer = realloc(buffer, buffer_size);

            if (new_buffer == NULL) {
                printf("Memory reallocation failed.\n");
                free(buffer);
                free_commands(head);
                return 1;
            }

            buffer = new_buffer;

            printf("Buffer resized to %zu bytes.\n", buffer_size);
        }

        add_command(&head, buffer);
    }

    display_commands(head);

    /* Release allocated memory */
    free(buffer);
    free_commands(head);

    printf("\nAll allocated memory has been released.\n");

    return 0;
}
