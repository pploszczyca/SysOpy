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

void splitTest(){
    char **test = (char **)calloc(2, sizeof(char *));
    test[0] = calloc(30, sizeof(char));
    test[1] = calloc(30, sizeof(char));
    strcpy(test[0], "file1.txt:file2.txt");
    strcpy(test[1], "file3.txt:file4.txt");

    char **tmp = splitFileNames(test, 2);
    // splitFileNames(test, 2);

    for(int i = 0; i < 4; i++){
        printf("%s\n", tmp[i]);
    }

    freeCharArray(test, 2);
    freeCharArray(tmp, 4);
}


void fileTest(){
    FILE *firstFile;
    FILE *secondFile;
    int n = 1;
    struct rowMergedFile **mainArray = makeMainTable(n);

    firstFile = openFileToRead("file1.txt");
    secondFile = openFileToRead("file2.txt");

    addFilesToArray(firstFile, secondFile, mainArray, 0);

    // print3DArray(mainArray, n);

    FILE * tmp_file = mergeArrayToTemporaryFile(mainArray, 0);

    removeOneLineFromBlock(mainArray, 0, 2);
    print3DArray(mainArray, n);
    printf("\n");

    // removeOneBlock(mainArray, 0);

    // printf("%d", tmp_file_id);
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