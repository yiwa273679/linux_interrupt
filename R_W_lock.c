#include <pthread.h>
#include <stdio.h>

#define NUM_THREADS 3

int data = 0;
pthread_rwlock_t rwlock;

void *reader(void *arg) {
    int i;
    for (i = 0; i < 5; i++) {
        pthread_rwlock_rdlock(&rwlock);
        printf("Reader %d got read lock\n", (int) arg);
        printf("Reader %d read data = %d\n", (int) arg, data);
        pthread_rwlock_unlock(&rwlock);
        printf("Reader %d released read lock\n", (int) arg);
    }
    pthread_exit(NULL);
}

void *writer(void *arg) {
    int i;
    for (i = 0; i < 5; i++) {
        pthread_rwlock_wrlock(&rwlock);
        printf("Writer %d got write lock\n", (int) arg);
        data += 1;
        printf("Writer %d modified data to %d\n", (int) arg, data);
        pthread_rwlock_unlock(&rwlock);
        printf("Writer %d released write lock\n", (int) arg);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int i;
    pthread_t threads[NUM_THREADS];

    /* Initialize the read-write lock */
    pthread_rwlock_init(&rwlock, NULL);

    /* Create reader threads */
    for (i = 0; i < NUM_THREADS - 1; i++) {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&threads[i], NULL, reader, arg);
    }

    /* Create writer thread */
    int *arg = malloc(sizeof(*arg));
    *arg = i;
    pthread_create(&threads[i], NULL, writer, arg);

    /* Wait for all threads to finish */
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Destroy the read-write lock */
    pthread_rwlock_destroy(&rwlock);

    return 0;
}
