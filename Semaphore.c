#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t sem;  // 信号量

void *thread1(void *arg)
{
    printf("Thread 1 start\n");
    sem_wait(&sem);  // 获取信号量
    printf("Thread 1 run\n");
    sem_post(&sem);  // 释放信号量
    printf("Thread 1 end\n");
    return NULL;
}

void *thread2(void *arg)
{
    printf("Thread 2 start\n");
    sem_wait(&sem);  // 获取信号量
    printf("Thread 2 run\n");
    sem_post(&sem);  // 释放信号量
    printf("Thread 2 end\n");
    return NULL;
}

int main()
{
    pthread_t tid1, tid2;
    sem_init(&sem, 0, 1);  // 初始化信号量 0 means inter-thread shared(1 means inter-process shared) 1 means just have one thread can access resource.
    pthread_create(&tid1, NULL, thread1, NULL);
    pthread_create(&tid2, NULL, thread2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    sem_destroy(&sem);  // 销毁信号量
    return 0;
}

