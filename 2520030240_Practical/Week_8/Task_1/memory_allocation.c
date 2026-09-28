#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int i;

    printf("===== WEEK 8 - TASK 1 =====\n");
    printf("Dynamic Memory Allocation Analysis\n\n");

    /* malloc() */
    printf("1. malloc() demonstration\n");

    int *malloc_ptr = malloc(5 * sizeof(int));

    if (malloc_ptr == NULL)
    {
        printf("malloc() failed.\n");
        return 1;
    }

    for (i = 0; i < 5; i++)
    {
        malloc_ptr[i] = (i + 1) * 10;
    }

    printf("Allocated 5 integers using malloc(): ");

    for (i = 0; i < 5; i++)
    {
        printf("%d ", malloc_ptr[i]);
    }

    printf("\n");

    /* calloc() */
    printf("\n2. calloc() demonstration\n");

    int *calloc_ptr = calloc(5, sizeof(int));

    if (calloc_ptr == NULL)
    {
        printf("calloc() failed.\n");
        free(malloc_ptr);
        return 1;
    }

    printf("Allocated 5 integers using calloc(): ");

    for (i = 0; i < 5; i++)
    {
        printf("%d ", calloc_ptr[i]);
    }

    printf("\n");
    printf("calloc() initializes allocated memory to zero.\n");

    /* realloc() */
    printf("\n3. realloc() demonstration\n");

    int *realloc_ptr = realloc(malloc_ptr, 10 * sizeof(int));

    if (realloc_ptr == NULL)
    {
        printf("realloc() failed.\n");
        free(malloc_ptr);
        free(calloc_ptr);
        return 1;
    }

    malloc_ptr = realloc_ptr;

    for (i = 5; i < 10; i++)
    {
        malloc_ptr[i] = (i + 1) * 10;
    }

    printf("Memory resized from 5 integers to 10 integers.\n");
    printf("Values after realloc(): ");

    for (i = 0; i < 10; i++)
    {
        printf("%d ", malloc_ptr[i]);
    }

    printf("\n");

    /* free() */
    printf("\n4. free() demonstration\n");

    free(malloc_ptr);
    free(calloc_ptr);

    printf("Allocated memory successfully released using free().\n");

    printf("\nMemory allocation experiment completed.\n");

    return 0;
}

