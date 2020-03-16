#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

int su1, su2, su3;
sem_t se_one, se_tow;
pthread_mutex_t mu_t;
void *mutex_plus(void *arg);
void *sem_sub(void *arg);
void *sem_sub1(void *arg);
int num;

int main()
{
    pthread_t pm_id[100], ps_id[2];
    pthread_mutex_init(&mu_t, NULL);
    sem_init(&se_one, 0, 0);
    sem_init(&se_tow, 0, 1);
    int i;
    for (i = 0; i < 100; ++i) {
        pthread_create(&(pm_id[i]), NULL, mutex_plus, (void *)&i);
        su3+=i;
    }
    int k = 5;
    pthread_create(&(ps_id[0]), NULL, sem_sub, (void *)&k);
    pthread_create(&(ps_id[1]), NULL, sem_sub1, (void *)&k);
    for (int i = 0; i < 100; ++i){
        pthread_join(pm_id[i], NULL);
    }
    pthread_join(ps_id[0], NULL);
    pthread_join(ps_id[1], NULL);
    pthread_mutex_destroy(&mu_t);
    sem_destroy(&se_one);
    sem_destroy(&se_tow);
    printf("%d %d %d\n", su1, su2, su3);
    return 0;
}

void *mutex_plus(void *arg) {
    int s = *(int *)arg;
        pthread_mutex_lock(&mu_t);
    for (int i = 0; i < s; ++i) {
        su1+=1;
    }
        pthread_mutex_unlock(&mu_t);
    return NULL;
}

void *sem_sub(void *arg) {
    int s = *(int *)arg;
    for (int i = 0; i < s; ++i) {
       fputs("INPUT:", stdout);
        sem_wait(&se_tow);
        scanf("%d",&num);
        sem_post(&se_one);
    }
    return NULL;
}

void *sem_sub1(void *arg)  {
    int s = *(int *)arg;
    for (int i = 0; i < s; ++i) {
//        pthread_mutex_lock(mu_t);
        sem_wait(&se_one);
        su2 -= num;
        sem_post(&se_tow);
    }
    return NULL;
}
