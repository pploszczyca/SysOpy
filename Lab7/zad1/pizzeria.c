#include "common.h"

void initArray(char *filename, int size){
    destroy_memory_block(filename);
    int *array = attach_memory_block(filename, size);

    for(int i = 0; i < size; i++)   array[i] = EMPTY;

    detach_memory_block(filename);
}

void initSemaphore(char *filename, int startValue){
    destroy_semaphore(filename);
    int semaphore_id = get_semaphore(filename);

    union semun arg;

    arg.val = 1; 
    semctl(semaphore_id, FILE_OPERATION_SEMAPHORE, SETVAL, arg);

    arg.val = startValue; 
    semctl(semaphore_id, COUTING_SEMAPHORE, SETVAL, arg);

    arg.val = 0; 
    semctl(semaphore_id, TABLES_EXTRA_COUNTING, SETVAL, arg);
}

void runPrograms(char *programPath, int n){
    for(int i = 0; i < n; i++){
        if(fork() == 0){
            execl(programPath, programPath, NULL);
            exit(0);
        }
    }
}

int main(int argc, char *argv[]){       // argv[1] - number of chefs, argv[2] - number of deliver men
    initArray(OVENS_KEY, N_OF_OVENS);
    initArray(TABLES_KEY, N_OF_TABLES);

    initSemaphore(OVENS_KEY, N_OF_OVENS);
    initSemaphore(TABLES_KEY, N_OF_TABLES);

    runPrograms("./chef", atoi(argv[1]));
    runPrograms("./deliver_man", atoi(argv[2]));

    while(wait(NULL) > 0);
    destroy_memory_block(OVENS_KEY);
    destroy_memory_block(TABLES_KEY);
    destroy_semaphore(OVENS_KEY);
    destroy_semaphore(TABLES_KEY);
    return 0;
}