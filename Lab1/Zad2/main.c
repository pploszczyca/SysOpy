#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "block_library.h"

void free3dArray(struct rowMergedFile ** array, int n_array){
    if(array == NULL){
        return;
    }

    for(int i = 0; i < n_array; i++){
        if(array[i] != NULL){
            for(int j = 0; j <  array[i]->n_rows; j++){
                free(array[i]->rows[j]);
            }
            free(array[i]);
        }
    }
    free(array);
}

void freeCharArray(char **array, int n_array){
    for(int i = 0; i < n_array; i++){
        free(array[i]);
    }

    free(array);
}


int main(int argc, char *argv[]){
    struct rowMergedFile ** mainArray = NULL;
    int n_mainArray = 0;

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "create_table") == 0){
            // free3dArray(mainArray, n_mainArray);
            n_mainArray = atoi(argv[++i]);
            mainArray = makeMainTable(n_mainArray);
        }
        else if(strcmp(argv[i], "merge_files") == 0){
            for(int j = 0; j < n_mainArray; j++){   // Zakładam, że ilość plików mergowanych jest taka sama, jak wielkość tablicy głównej
                char **tmpFileNames = splitTwoFileNames(argv[++i]);
                
                FILE *firstFile = openFileToRead(tmpFileNames[0]);
                FILE *secondFile = openFileToRead(tmpFileNames[1]);

                addFilesToArray(firstFile, secondFile, mainArray, j);

                fclose(firstFile);
                fclose(secondFile);
                freeCharArray(tmpFileNames, 2);
            }
        }
        else if(strcmp(argv[i], "remove_block") == 0){
            removeOneBlock(mainArray, atoi(argv[++i]));
        }
        else if(strcmp(argv[i], "remove_row") == 0){
            int block_index = atoi(argv[++i]);
            int row_index = atoi(argv[++i]);
            removeOneLineFromBlock(mainArray, block_index, row_index);
        }
        else{
            return 0;
        }
    }

    print3DArray(mainArray, n_mainArray);

    free3dArray(mainArray, n_mainArray);
    return 0;
}