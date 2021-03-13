#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "block_library.c"

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


int main(int argc, char *argv[]){
    FILE *firstFile;
    FILE *secondFile;
    int n = 1;
    struct rowMergedFile **mainArray = makeMainTable(n);

    firstFile = openFileToRead("file1.txt");
    secondFile = openFileToRead("file2.txt");

    addFilesToArray(firstFile, secondFile, mainArray, 0);

    // removeOneBlock(mainArray, 0);

    print3DArray(mainArray, n);

    // printf("%d", mergeArrayToTemporaryFile(mainArray, 0));

    fclose(firstFile);
    fclose(secondFile);



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

    free3dArray(mainArray, n);

    return 0;
}