#include <stdio.h>
#include <pthread.h>

pthread_mutex_t A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t B = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *arg) {
    int id = *(int *)arg;

    /* Both threads acquire resources in the same order: A, then B. */
    pthread_mutex_lock(&A);
    printf("Thread %d: Locked resource A\n", id);

    pthread_mutex_lock(&B);
    printf("Thread %d: Locked resource B\n", id);

    printf("Thread %d: Using both resources\n", id);

    pthread_mutex_unlock(&B);
    pthread_mutex_unlock(&A);

    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    if (pthread_create(&t1, NULL, worker, &id1) != 0 ||
        pthread_create(&t2, NULL, worker, &id2) != 0) {
        fprintf(stderr, "Failed to create threads\n");
        return 1;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&A);
    pthread_mutex_destroy(&B);

    printf("Deadlock prevented using consistent resource ordering.\n");
    return 0;
}
