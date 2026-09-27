#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 4096
#define MAX_COMMAND_LENGTH 256

int is_executable(const char *path)
{
    struct stat file_info;

    if (stat(path, &file_info) != 0)
    {
        return 0;
    }

    if (!S_ISREG(file_info.st_mode))
    {
        return 0;
    }

    return access(path, X_OK) == 0;
}

int find_command(const char *command, char *result)
{
    char *path_variable;
    char *path_copy;
    char *directory;

    path_variable = getenv("PATH");

    if (path_variable == NULL)
    {
        printf("PATH variable is not set.\n");
        return 0;
    }

    path_copy = malloc(strlen(path_variable) + 1);

    if (path_copy == NULL)
    {
        perror("malloc failed");
        return 0;
    }

    strcpy(path_copy, path_variable);

    directory = strtok(path_copy, ":");

    while (directory != NULL)
    {
        snprintf(result, MAX_PATH_LENGTH, "%s/%s",
                 directory, command);

        if (is_executable(result))
        {
            free(path_copy);
            return 1;
        }

        directory = strtok(NULL, ":");
    }

    free(path_copy);
    return 0;
}

int main()
{
    char command[MAX_COMMAND_LENGTH];
    char result[MAX_PATH_LENGTH];

    printf("===== WEEK 7 - TASK 2 =====\n");
    printf("PATH Command Resolution\n\n");

    printf("Enter command: ");

    if (fgets(command, sizeof(command), stdin) == NULL)
    {
        return 1;
    }

    command[strcspn(command, "\n")] = '\0';

    if (strlen(command) == 0)
    {
        printf("Error: Empty command.\n");
        return 1;
    }

    printf("\nCommand: %s\n", command);

    printf("Searching PATH directories...\n");

    if (find_command(command, result))
    {
        printf("Executable found.\n");
        printf("Full path: %s\n", result);
    }
    else
    {
        printf("Command not found in PATH.\n");
    }

    return 0;
}

