
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ITERATIONS 1000000

long counter = 0;

void *increment(void *arg) {
    (void)arg;

    for (int i = 0; i < ITERATIONS; i++) {
        counter++;
    }

    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment, NULL) != 0) {
            fprintf(stderr, "Failed to create thread\n");
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Failed to join thread\n");
            return 1;
        }
    }

    printf("Expected counter: %d\n", NUM_THREADS * ITERATIONS);
    printf("Actual counter:   %ld\n", counter);

    return 0;
}
