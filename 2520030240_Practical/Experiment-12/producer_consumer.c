
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define ITEMS_TO_PRODUCE 20

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

sem_t empty_slots;
sem_t filled_slots;
pthread_mutex_t buffer_lock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg) {
    (void)arg;

    for (int item = 1; item <= ITEMS_TO_PRODUCE; item++) {
        sem_wait(&empty_slots);

        pthread_mutex_lock(&buffer_lock);
        buffer[in] = item;
        printf("Produced: %d\n", item);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&buffer_lock);

        sem_post(&filled_slots);

    }

    return NULL;
}

void *consumer(void *arg) {
    (void)arg;

    for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
        sem_wait(&filled_slots);

        pthread_mutex_lock(&buffer_lock);
        int item = buffer[out];
        printf("Consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&buffer_lock);

        sem_post(&empty_slots);

    }

    return NULL;
}

int main(void) {
    pthread_t producer_thread, consumer_thread;

    if (sem_init(&empty_slots, 0, BUFFER_SIZE) == -1 ||
        sem_init(&filled_slots, 0, 0) == -1) {
        perror("sem_init");
        return 1;
    }

    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0) {
        fprintf(stderr, "Producer thread creation failed\n");
        return 1;
    }

    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0) {
        fprintf(stderr, "Consumer thread creation failed\n");
        return 1;
    }

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);
    pthread_mutex_destroy(&buffer_lock);

    puts("Producer-consumer completed.");
    return 0;
}
