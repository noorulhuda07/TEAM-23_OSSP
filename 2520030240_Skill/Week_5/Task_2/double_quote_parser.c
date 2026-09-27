#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 500
#define MAX_TOKENS 50
#define MAX_TOKEN_LENGTH 200

void expand_variable(const char *input, int *index,
                     char *token, int *token_length)
{
    char variable_name[100];
    int var_length = 0;

    (*index)++;

    /* Read variable name */
    while (input[*index] != '\0' &&
           (isalnum((unsigned char)input[*index]) ||
            input[*index] == '_'))
    {
        if (var_length < 99)
        {
            variable_name[var_length++] = input[*index];
        }

        (*index)++;
    }

    variable_name[var_length] = '\0';

    /* Get environment variable */
    char *value = getenv(variable_name);

    if (value != NULL)
    {
        for (int i = 0; value[i] != '\0'; i++)
        {
            if (*token_length < MAX_TOKEN_LENGTH - 1)
            {
                token[(*token_length)++] = value[i];
            }
        }
    }

    (*index)--;
}

void parse_double_quotes(char *input)
{
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];

    int token_count = 0;
    int token_length = 0;
    int inside_double_quotes = 0;

    char current_token[MAX_TOKEN_LENGTH];

    memset(current_token, 0, sizeof(current_token));

    for (int i = 0; input[i] != '\0'; i++)
    {
        char ch = input[i];

        /* Handle double quote */
        if (ch == '"')
        {
            inside_double_quotes = !inside_double_quotes;
            continue;
        }

        /* Variable expansion */
        if (ch == '$')
        {
            expand_variable(input, &i,
                            current_token, &token_length);
            continue;
        }

        /* Handle spaces outside quotes */
        if ((ch == ' ' || ch == '\t') && !inside_double_quotes)
        {
            if (token_length > 0)
            {
                current_token[token_length] = '\0';

                strcpy(tokens[token_count], current_token);
                token_count++;

                token_length = 0;
                memset(current_token, 0, sizeof(current_token));
            }

            continue;
        }

        /* Store normal character */
        if (token_length < MAX_TOKEN_LENGTH - 1)
        {
            current_token[token_length++] = ch;
        }
    }

    /* Check unmatched quote */
    if (inside_double_quotes)
    {
        printf("Syntax Error: Unmatched double quote.\n");
        return;
    }

    /* Store final token */
    if (token_length > 0)
    {
        current_token[token_length] = '\0';

        strcpy(tokens[token_count], current_token);
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

    printf("===== WEEK 5 - TASK 2 =====\n");
    printf("Double Quote Parser\n\n");

    printf("Enter command: ");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    parse_double_quotes(input);

    return 0;
}


