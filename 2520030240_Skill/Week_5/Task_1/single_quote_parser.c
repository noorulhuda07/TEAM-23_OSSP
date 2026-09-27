#include <stdio.h>
#include <string.h>

#define MAX_INPUT 200
#define MAX_TOKENS 20
#define MAX_TOKEN_LENGTH 100

void parse_single_quotes(char *input)
{
    char tokens[MAX_TOKENS][MAX_TOKEN_LENGTH];

    int token_count = 0;
    int token_length = 0;
    int inside_single_quotes = 0;

    for (int i = 0; input[i] != '\0'; i++)
    {
        char ch = input[i];

        /* Handle single quote */
        if (ch == '\'')
        {
            inside_single_quotes = !inside_single_quotes;
            continue;
        }

        /* Handle spaces outside quotes */
        if ((ch == ' ' || ch == '\t') && !inside_single_quotes)
        {
            if (token_length > 0)
            {
                tokens[token_count][token_length] = '\0';
                token_count++;
                token_length = 0;
            }

            continue;
        }

        /* Store character in current token */
        if (token_length < MAX_TOKEN_LENGTH - 1)
        {
            tokens[token_count][token_length] = ch;
            token_length++;
        }
    }

    /* Check for unmatched single quote */
    if (inside_single_quotes)
    {
        printf("Syntax Error: Unmatched single quote.\n");
        return;
    }

    /* Store last token */
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

    printf("Enter command: ");

    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    parse_single_quotes(input);

    return 0;
}
