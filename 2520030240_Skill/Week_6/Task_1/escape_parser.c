#include <stdio.h>
#include <string.h>

#define MAX_INPUT 500
#define MAX_TOKENS 50
#define MAX_TOKEN_LENGTH 200

void parse_escaped_input(char *input)
{
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];

    int token_count = 0;
    int token_length = 0;
    int escaped = 0;

    for (int i = 0; input[i] != '\0'; i++)
    {
        char ch = input[i];

        /* Handle escape character */
        if (ch == '\\' && !escaped)
        {
            escaped = 1;
            continue;
        }

        /* Handle escaped character */
        if (escaped)
        {
            if (token_length < MAX_TOKEN_LENGTH - 1)
            {
                tokens[token_count][token_length++] = ch;
            }

            escaped = 0;
            continue;
        }

        /* Handle spaces outside escape sequences */
        if (ch == ' ' || ch == '\t')
        {
            if (token_length > 0)
            {
                tokens[token_count][token_length] = '\0';
                token_count++;
                token_length = 0;
            }

            continue;
        }

        /* Store normal character */
        if (token_length < MAX_TOKEN_LENGTH - 1)
        {
            tokens[token_count][token_length++] = ch;
        }
    }

    /* Handle trailing escape character */
    if (escaped)
    {
        printf("Syntax Error: Trailing escape character.\n");
        return;
    }

    /* Store final token */
    if (token_length > 0)
    {
        tokens[token_count][token_length] = '\0';
        token_count++;
    }

    printf("\nParsed Tokens:\n");

    for (int i = 0; i < token_count; i++)
    {
        printf("Token %d: %s\n", i + 1, tokens[i]);
    }

    if (token_count == 0)
    {
        printf("Empty command.\n");
    }
}

int main()
{
    char input[MAX_INPUT];

    printf("===== WEEK 6 - TASK 1 =====\n");
    printf("Escape Sequence Parser\n\n");

    printf("Enter command: ");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    parse_escaped_input(input);

    return 0;
}
