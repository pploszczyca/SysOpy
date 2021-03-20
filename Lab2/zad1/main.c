#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// LIB VERSION

int printOneLineWithLib(FILE *file){
    char buffer;
    int isReaded;       // Flaga informująca, czy można dalej czytać z pliku

    while((isReaded = fread(&buffer, sizeof(char), 1, file)) == 1 && buffer != '\n'){
        printf("%s", &buffer);
    }

    printf("\n");
    return isReaded;
}

FILE *openFileWithLibToRead(char *fileName){
    FILE *file = fopen(fileName, "r");
    if(file == NULL){
        printf("Cann't open %s\n", fileName);
        exit(1);
    }

    return file;
}

void printWithLibOpen(char *firstFileName, char *secondFileName){
    FILE *firstFile = openFileWithLibToRead(firstFileName);
    FILE *secondFile = openFileWithLibToRead(secondFileName);
    int isReadingFirstFile;
    int isReadingSecondFile;

    do{
        isReadingFirstFile = printOneLineWithLib(firstFile);
        isReadingSecondFile = printOneLineWithLib(secondFile);
    }while(isReadingFirstFile || isReadingSecondFile);

    fclose(firstFile);
    fclose(secondFile);
}

// SYS VERSION

int printOneLineWithSys(int file){
    char buffer;
    int isReaded;       // Flaga informująca, czy można dalej czytać z pliku

    while((isReaded = read(file, &buffer, sizeof(char))) == 1 && buffer != '\n'){
        printf("%s", &buffer);
    }

    printf("\n");
    return isReaded;
}

int openFileWithSysToRead(char *fileName){
    int file = open(fileName, O_RDONLY);
    if(file == NULL){
        printf("Cann't open %s\n", fileName);
        exit(1);
    }

    return file;
}

void printWithSysOpen(char *firstFileName, char *secondFileName){
    int firstFile = openFileWithSysToRead(firstFileName);
    int secondFile = openFileWithSysToRead(secondFileName);
    int isReadingFirstFile;
    int isReadingSecondFile;

    do{
        isReadingFirstFile = printOneLineWithSys(firstFile);
        isReadingSecondFile = printOneLineWithSys(secondFile);
    }while(isReadingFirstFile || isReadingSecondFile);

    close(firstFile);
    close(secondFile);
}

// MAIN

double calculateDiffrenceBetweenTimes(clock_t startTime, clock_t endTime){
    return (double) (endTime - startTime) / CLOCKS_PER_SEC;
}

void printTimes(double libTime, double sysTime){
    printf("TIMES:\n LIBRARY: %f\n SYSTEM: %f\n\n", libTime, sysTime);
}

int main(int argc, char *argv[]){
    clock_t startTimeSys, endTimeSys, startTimeLib, endTimeLib;
    char *firstFileName;
    char *secondFileName;

    if(argc == 1){
        scanf("%s\n", firstFileName);
        scanf("%s\n", secondFileName);
    } else if (argc == 3){
        firstFileName = calloc(strlen(argv[1]), sizeof(char));
        strcpy(firstFileName, argv[1]);
        secondFileName = calloc(strlen(argv[2]), sizeof(char));
        strcpy(secondFileName, argv[2]);
    } else {
        printf("Bad arguments!\n");
        return 0;
    }
    
    startTimeLib = clock();
    printWithLibOpen(firstFileName, secondFileName);
    endTimeLib = clock();

    startTimeSys = clock();
    printWithSysOpen(firstFileName, secondFileName);
    endTimeSys = clock();

    printTimes(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));

    free(firstFileName);
    free(secondFileName);
    return 0;
}