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
#include <sys/types.h> 

#define N_OF_OVENS 5
#define N_OF_TABLES 5

#define OVENS_KEY "chef.c"
#define TABLES_KEY "deliver_man.c"

#define FILE_OPERATION_SEMAPHORE 0
#define COUTING_SEMAPHORE 1
#define TABLES_EXTRA_COUNTING 2

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

// FUNCTIONS FOR SEMAPHORE

union semun {
    int val;               /* used for SETVAL only */
    struct semid_ds *buf;  /* used for IPC_STAT and IPC_SET */
    ushort *array;         /* used for GETALL and SETALL */
};

static int get_semaphore(char *filename_of_semaphore){
    key_t key = ftok(filename_of_semaphore, 2);
    if(key == -1)   return -1;

    return semget(key, 3, 0777 | IPC_CREAT);        // first for reading data from array, second for couting elements in array
}

void make_semaphore_operation(char *filename_of_semaphore, int semaphore_number, int operation){
    int semaphore_id = get_semaphore(filename_of_semaphore);

    if(semaphore_id == -1){
        printf("Semaphore ID error\n");
        return;
    }

    struct sembuf buffer;
    buffer.sem_num = semaphore_number;
    buffer.sem_op = operation;
    buffer.sem_flg = 0;

    semop(semaphore_id, &buffer, 1);
}

int destroy_semaphore(char *filename_of_semaphore){
    int semaphore_id = get_semaphore(filename_of_semaphore);

    semctl(semaphore_id, 0, IPC_RMID);
    
}

// PIZZA COUNTING

int calculatePizzas(char *filename, int n){
    int semaphore_id = get_semaphore(filename);
    int value = semctl(semaphore_id, COUTING_SEMAPHORE, GETVAL);
    
    return n - value;
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