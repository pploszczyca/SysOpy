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
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>


#define N_OF_OVENS 5
#define N_OF_TABLES 5

#define OVENS_KEY "chef.c"
#define TABLES_KEY "deliver_man.c"

#define OVENS_FILE_OPERATION_SEMAPHORE "/ovens_file_operation_semaphore"
#define OVENS_COUTING_SEMAPHORE "/ovens_couting_semaphore"

#define TABLES_FILE_OPERATION_SEMAPHORE "/tables_file_operation_semaphore"
#define TABLES_COUTING_SEMAPHORE "/tables_couting_semaphore"
#define TABLES_TABLES_EXTRA_COUNTING "/tables_tables_extra_counting_semaphore"

#define POST 1
#define WAIT -1

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

sem_t * create_semaphore(char *name_of_semaphore, int start_value){
    return sem_open(name_of_semaphore, O_CREAT, 0660, start_value);
}

sem_t * get_semaphore(char *name_of_semaphore){
    return sem_open(name_of_semaphore, 0);
}

void make_semaphore_operation(char *filename_of_semaphore, int operation){
    sem_t * semaphore_id = get_semaphore(filename_of_semaphore);

    if(semaphore_id == SEM_FAILED){
        printf("Semaphore ID error\n");
        return;
    }

    if(operation == WAIT){
        sem_wait(semaphore_id);
    } else if (operation == POST){
        sem_post(semaphore_id);
    }

    sem_close(semaphore_id);
}

int destroy_semaphore(char *name_of_semaphore){
    sem_unlink(name_of_semaphore);
}

// PIZZA COUNTING

int calculatePizzas(char *filename, int n){
    sem_t * semaphore_id = get_semaphore(filename);
    int value;

    sem_getvalue(semaphore_id, &value);

    sem_close(semaphore_id);
    
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