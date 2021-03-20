#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "block_library.h"

void free3dArray(struct rowMergedFile ** array, int n_array){
    for(int i = 0; i < n_array; i++){
        for(int j = 0; j <  array[i]->n_rows; j++){
            free(array[i]->rows[j]);
        }
        free(array[i]);
    }
    free(array);
}

void freeCharArray(char **array, int n_array){
    for(int i = 0; i < n_array; i++){
        free(array[i]);
    }

    free(array);
}

void fileTest(){
    FILE *firstFile;
    FILE *secondFile;
    int n = 1;
    struct rowMergedFile **mainArray = makeMainTable(n);

    firstFile = openFileToRead("file1.txt");
    secondFile = openFileToRead("file2.txt");

    addFilesToArray(firstFile, secondFile, mainArray, 0);

    FILE * tmp_file = mergeArrayToTemporaryFile(mainArray, 0);

    removeOneLineFromBlock(mainArray, 0, 2);
    print3DArray(mainArray, n);
    printf("\n");

    readFromTemporaryFileToArray(tmp_file, mainArray, 0);

    print3DArray(mainArray, n);

    fclose(tmp_file);
    fclose(firstFile);
    fclose(secondFile);

    free3dArray(mainArray, n);
}


int main(int argc, char *argv[]){
    fileTest();
    return 0;
}