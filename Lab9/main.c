#include "stdlib.h"
#include <pthread.h>
#include <sys/time.h>
#include <stdio.h>

#define N_OF_ELFS 10
#define N_OF_REINDEERS 9
#define N_OF_SANTA_CLAUS 1
#define MAX_ELFS_IN_QUEUE 3
#define MAX_TRIPS_TO_DELIVER_GIFTS 3
#define N (N_OF_ELFS + N_OF_REINDEERS + N_OF_SANTA_CLAUS)       // Amount of all threads in program

int amount_of_elfs_waiting = 0;
int amount_of_ready_reindeers = 0;
int elfs_queue_ids[MAX_ELFS_IN_QUEUE];

// MUTEX-es
pthread_mutex_t elfs_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reindeers_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;

// Conditions
pthread_cond_t wake_santa_claus_cond;
pthread_cond_t reindeers_back_cond;
pthread_cond_t elfs_back_from_santa_cond;

void *santa_claus(void *arg) {
    int id = *(int *)arg;
    int amount_of_delivering_gifts = 0;

    free(arg);

    while(amount_of_delivering_gifts < MAX_TRIPS_TO_DELIVER_GIFTS) {
        while(amount_of_ready_reindeers < N_OF_REINDEERS && amount_of_elfs_waiting < MAX_ELFS_IN_QUEUE){
            pthread_cond_wait(&wake_santa_claus_cond, &santa_mutex);
        }

        if(amount_of_ready_reindeers == N_OF_REINDEERS) {
            printf("\nMikołaj: dostarczam zabawki!\n\n");
            sleep(rand()%3+2);
            pthread_mutex_lock(&reindeers_mutex);
            amount_of_ready_reindeers = 0;
            amount_of_delivering_gifts++;
            pthread_mutex_unlock(&reindeers_mutex);
            pthread_cond_broadcast(&reindeers_back_cond);
        }

        if(amount_of_elfs_waiting == MAX_ELFS_IN_QUEUE) {
            printf("\nMikołaj: naprawiam problemy elfów: %d %d %d", elfs_queue_ids[0], elfs_queue_ids[1], elfs_queue_ids[2]);
            printf("\n\n");

            sleep(rand()%2+1);

            pthread_mutex_lock(&elfs_mutex);
            for(int i = 0; i < MAX_ELFS_IN_QUEUE; i++)  elfs_queue_ids[i] = -1;

            amount_of_elfs_waiting = 0;
            pthread_mutex_unlock(&elfs_mutex);
            pthread_cond_broadcast(&elfs_back_from_santa_cond);
        }

        printf("\nMikołaj: zasypiam\n\n");
    }

    printf("\nMikołaj: kończe pracę na dziś\n\n");
    return;
}

void *elf(void *arg) {
    int id = *(int *)arg;
    free(arg);

    while(1) {
        sleep(rand()%4 + 2);

        pthread_mutex_lock(&elfs_mutex);

        while(amount_of_elfs_waiting == MAX_ELFS_IN_QUEUE){
            printf("Elf: czekam na powrót elfów, ID: %d\n", id);
            pthread_cond_wait(&elfs_back_from_santa_cond, &elfs_mutex);
        }

        elfs_queue_ids[amount_of_elfs_waiting] = id;
        amount_of_elfs_waiting++;
        printf("Elf: czeka %d elfów na Mikołaja, ID: %d\n", amount_of_elfs_waiting, id);

        if(amount_of_elfs_waiting == MAX_ELFS_IN_QUEUE)
            pthread_cond_signal(&wake_santa_claus_cond);

        pthread_cond_wait(&elfs_back_from_santa_cond, &elfs_mutex);

        pthread_mutex_unlock(&elfs_mutex);
    }

    return;
}

void *reindeer(void *arg) {
    int id = *(int *)arg;
    free(arg);

    while(1) {
        sleep(rand()%6 + 5);

        pthread_mutex_lock(&reindeers_mutex);
        amount_of_ready_reindeers++;
        printf("Renifer: czeka %d reniferów na Mikołaja, ID: %d\n", amount_of_ready_reindeers, id);

        if(amount_of_ready_reindeers == N_OF_REINDEERS)
            pthread_cond_signal(&wake_santa_claus_cond);
        
        pthread_cond_wait(&reindeers_back_cond, &reindeers_mutex);

        pthread_mutex_unlock(&reindeers_mutex);
    }

    return;
}

int main(int argc, char const *argv[]) {
    pthread_t *threads = calloc(N, sizeof(pthread_t));

    pthread_cond_init(&wake_santa_claus_cond, NULL);
    pthread_cond_init(&reindeers_back_cond, NULL);
    pthread_cond_init(&elfs_back_from_santa_cond, NULL);

    srand(time(NULL));

    // Run all threads
    for(int i = 0; i < N; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        if(i < N_OF_SANTA_CLAUS)
            pthread_create(&threads[i], NULL, santa_claus, id);
        else if(i < N_OF_SANTA_CLAUS + N_OF_ELFS)
            pthread_create(&threads[i], NULL, elf, id);
        else
            pthread_create(&threads[i], NULL, reindeer, id);
    }
    
    pthread_join(threads[0], NULL);     // Waiting for end of santa claus thread

    for(int i = 1 ; i < N; i++)     // Cancel elfs and reindeers threads
        pthread_cancel(threads[i]);

    pthread_cond_destroy(&wake_santa_claus_cond);
    pthread_cond_destroy(&reindeers_back_cond);
    pthread_cond_destroy(&elfs_back_from_santa_cond);
    free(threads);
    return 0;
}
