#include "common.h"

void initArray(char *filename, int size){
    int *array = attach_memory_block(filename, size);

    for(int i = 0; i < size; i++)   array[i] = EMPTY;

    detach_memory_block(filename);
}

int main(int argc, char *argv[]){       // argv[1] - number of chefs, argv[2] - number of deliver men
    // Remove previous memory
    destroy_memory_block(OVENS_SHARED_MEMORY_KEY);
    destroy_memory_block(TABLES_SHARED_MEMORY_KEY);

    initArray(OVENS_SHARED_MEMORY_KEY, N_OF_OVENS);
    initArray(TABLES_SHARED_MEMORY_KEY, N_OF_TABLES);

    // int n_of_chefs = atoi(argv[1]);
    // int n_of_deliver_men = atoi(argv[2]);


    // while(wait(NULL) > 0);
    // destroy_memory_block(OVENS_SHARED_MEMORY_KEY);
    // destroy_memory_block(TABLES_SHARED_MEMORY_KEY);
    return 0;
}