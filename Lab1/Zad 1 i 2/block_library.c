#include "block_library.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct rowMergedFile{
    int n_rows;
    char **rows;
};

struct rowMergedFile ** makeMainTable(size_t n_elements){
    return (struct rowMergedFile **)calloc(n_elements, sizeof(struct rowMergedFile *));
}

FILE* openFileToRead(char *nameOfTheFile){
    FILE *fp = fopen(nameOfTheFile, "r");

    if(fp == NULL){
        perror("Unable to open file!");
        exit(1);
    }

    return fp;
}

int getFileLines(FILE *fp){
    int count = 0;
    char c;
    ssize_t read;
    char * line = NULL;
    size_t len = 0;

    while((read = getline(&line, &len, fp)) != -1){
        count++;
    }

    fseek(fp, 0, 0);

    return count*2;
}

void addFileToArray(FILE *fileToAdd, struct rowMergedFile **arrayOfFiles, int index, int isEven){
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;

    while ((read = getline(&line, &len, fileToAdd)) != -1) {
        arrayOfFiles[index]->rows[i*2+isEven] = (char *)calloc(len, sizeof(char));
        strcpy(arrayOfFiles[index]->rows[i*2+isEven], line);

        i++;
    }
}

void addFilesToArray(FILE *firstFile, FILE *secondFile, struct rowMergedFile **arrayOfFiles, int index){
    arrayOfFiles[index] = (struct rowMergedFile *)calloc(1, sizeof(struct rowMergedFile));
    arrayOfFiles[index]->n_rows = getFileLines(firstFile);

    arrayOfFiles[index]->rows = (char **)calloc(arrayOfFiles[index]->n_rows, sizeof(char *));

    addFileToArray(firstFile, arrayOfFiles, index, 0);
    addFileToArray(secondFile, arrayOfFiles, index, 1);
}

// char * mergeToOneChar(struct rowMergedFile **arrayOfFiles, int index){
//     char result[1000] = "";

//     for(int i = 0; i < arrayOfFiles[index]->n_rows; i++){
//         strcat(result, arrayOfFiles[index]->rows[i]);
//         printf("%s\n", result);
//     }

//     return result;
// }

FILE * mergeArrayToTemporaryFile(struct rowMergedFile **arrayOfFiles, int index){
    FILE *fh = tmpfile();   //tworzy plik tymczasowy

    for(int i = 0; i < arrayOfFiles[index]->n_rows; i++){
        fprintf(fh, arrayOfFiles[index]->rows[i]);
    }

    return fh;
}

void readFromTemporaryFileToArray(FILE* temp_file, struct rowMergedFile **arrayOfFiles, int index){
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;

    arrayOfFiles[index]->n_rows = getFileLines(temp_file)/2;

    arrayOfFiles[index]->rows = (char **)calloc(arrayOfFiles[index]->n_rows, sizeof(char *));

    rewind(temp_file);    // przewija plik na sam początek

    while ((read = getline(&line, &len, temp_file)) != -1) {
        arrayOfFiles[index]->rows[i] = (char *)calloc(len, sizeof(char));       //TU GDZIEŚ JEST BŁĄD, jak się zakomentuje tą i poniższą linijkę, to nie wywala błędu
        strcpy(arrayOfFiles[index]->rows[i], line);
        printf("%s", line);

        i++;
    }

}

void print3DArray(struct rowMergedFile ** array, int n_array){
    for(int i = 0; i < n_array; i++){
        printf("%d\n", i);
        for(int j = 0; j < array[i]->n_rows; j++){
            printf("%s", (array[i]->rows != NULL && array[i]->rows[j] != NULL) ? array[i]->rows[j] : "\n");
        }
    }
}

void removeOneLineFromBlock(struct rowMergedFile ** array, int blockIndex, int lineIndex){
    free(array[blockIndex]->rows[lineIndex]);
    array[blockIndex]->rows[lineIndex] = NULL;
}


void removeOneBlock(struct rowMergedFile ** array, int blockIndex){
    for(int j = 0; j <  array[blockIndex]->n_rows; j++){
        free(array[blockIndex]->rows[j]);
    }
    
    free(array[blockIndex]->rows);
    array[blockIndex]->rows = NULL;
}

char ** splitFileNames(char **notSplitFileNames, int n_fileNames){
    char **splitFileNames = (char **)calloc(n_fileNames*2, sizeof(char *));
    char delim[] = ":";
    char *ptr;

    for(int i = 0; i < n_fileNames; i++){
        ptr = strtok(notSplitFileNames[i], delim);
        splitFileNames[i*2] = calloc(sizeof(ptr)/sizeof(ptr[0]), sizeof(char));
        strcpy(splitFileNames[i*2], ptr);
        ptr = strtok(NULL, delim);
        splitFileNames[i*2+1] = calloc(sizeof(ptr)/sizeof(ptr[0]), sizeof(char));
        strcpy(splitFileNames[i*2+1], ptr);
    }

    return splitFileNames;
}