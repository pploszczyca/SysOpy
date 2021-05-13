#include "common.h"

void initArray(char *filename, int size){
    destroy_memory_block(filename);
    create_shated_block(filename, size);
    
    int *array = attach_memory_block(filename, size);

    for(int i = 0; i < size; i++)   array[i] = EMPTY;

    detach_memory_block(filename, size);
}

void initSemaphores(){
    int n = 5;
    char *semaphores[] = {OVENS_FILE_OPERATION_SEMAPHORE, OVENS_COUTING_SEMAPHORE, TABLES_FILE_OPERATION_SEMAPHORE, TABLES_COUTING_SEMAPHORE, TABLES_TABLES_EXTRA_COUNTING};
    int initValues[] = {1, N_OF_OVENS, 1, N_OF_TABLES, 0};

    for(int i = 0; i < n; i++){
        destroy_semaphore(semaphores[i]);
        sem_t * tmp = create_semaphore(semaphores[i], initValues[i]);
        
        if(tmp == SEM_FAILED){
            printf("CREATING ERROR\n");
            continue;
        }

        sem_close(tmp);
    }
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

    initSemaphores();

    runPrograms("./chef", atoi(argv[1]));
    runPrograms("./deliver_man", atoi(argv[2]));

    while(wait(NULL) > 0);
    destroy_memory_block(OVENS_KEY);
    destroy_memory_block(TABLES_KEY);
    return 0;
}