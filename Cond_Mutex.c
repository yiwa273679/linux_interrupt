#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];  // 缓冲区
int in = 0, out = 0;      // 缓冲区的读写指针

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   // 互斥锁
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;     // 条件变量

void *producer(void *arg)
{
    int i;
    for (i = 0; i < 100; i++) {
        pthread_mutex_lock(&mutex);  // 获取互斥锁
        while ((in + 1) % BUFFER_SIZE == out) {
            pthread_cond_wait(&cond, &mutex);  // 等待缓冲区有空间 wait consumer's pthread_cond_signal();
        }
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        printf("Produce: %d\n", i);
        pthread_cond_signal(&cond);  // 唤醒一个等待线程
        pthread_mutex_unlock(&mutex);  // 释放互斥锁
    }
    return NULL;
}

void *consumer(void *arg)
{
    int data, i;
    for (i = 0; i < 100; i++) {
        pthread_mutex_lock(&mutex);  // 获取互斥锁
        while (in == out) {
            pthread_cond_wait(&cond, &mutex);  // 等待缓冲区有数据 wait producer's pthread_cond_signal();
        }
        data = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consume: %d\n", data);
        pthread_cond_signal(&cond);  // 唤醒一个等待线程 go to producer's pthread_cond_wait();
        pthread_mutex_unlock(&mutex);  // 释放互斥锁
    }
    return NULL;
}

int main()
{
    pthread_t tid_producer, tid_consumer;
    pthread_create(&tid_producer, NULL, producer, NULL);
    pthread_create(&tid_consumer, NULL, consumer, NULL);
    pthread_join(tid_producer, NULL);
    pthread_join(tid_consumer, NULL);
    return 0;
}

