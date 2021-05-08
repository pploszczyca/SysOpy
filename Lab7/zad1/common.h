#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <inttypes.h>
#include <math.h>

#define N_OF_OVENS 5
#define N_OF_TABLES 5

#define OVENS_SHARED_MEMORY_KEY "chef.c"
#define TABLES_SHARED_MEMORY_KEY "deliver_man.c"

#define EMPTY -1

#define CURRENT_TIME_SIZE 100

char * getTimeAsString(){
    time_t rawtime;
    struct tm * timeinfo;
    char *timeInString;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    timeInString = asctime(timeinfo);
    timeInString[strlen(timeInString)-1] = '\0';

    return timeInString;
}

int calculatePizzas(int *array, int n){
    int ammount = 0;
    for(int i = 0; i < n; i++){
        if(array[i] != EMPTY){
            ammount++;
        }
    }

    return ammount;
}

// FUNCTIONS FOR SHARED MEMORY

static int get_shared_block(char *filename_of_memory, int size){        // if size == 0 get existed memory id else if size > 0 then make new memory
    key_t key = ftok(filename_of_memory, 0);
    if(key == -1)   return -1;

    return shmget(key, size*sizeof(int), 0777 | IPC_CREAT);
}

int *attach_memory_block(char *filename_of_memory, int size){
    int shared_block_id = get_shared_block(filename_of_memory, size);
    int *result;

    if(shared_block_id == -1)   return NULL;

    result = (int *) shmat(shared_block_id, NULL, 0);
    if(result == (int *)-1) return NULL;

    return result;
}

int detach_memory_block(int *block){
    return (shmdt(block) != -1);
}

int destroy_memory_block(char *filename_of_memory){
    int shared_block_id = get_shared_block(filename_of_memory, 0);

    if(shared_block_id == -1)   return NULL;

    return (shmctl(shared_block_id, IPC_RMID, NULL) != -1);
}