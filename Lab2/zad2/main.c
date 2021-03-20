#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FILE_MAX_NAME_SIZE 100
#define LINE_MAX_SIZE 256

// LIB VERSION

FILE *openFileWithLibToRead(char *fileName){
    FILE *file = fopen(fileName, "r");
    if(file == NULL){
        printf("Cann't open %s\n", fileName);
        exit(1);
    }

    return file;
}

void printWithLibOpen(char *fileName, char charToFind){
    FILE *file = openFileWithLibToRead(fileName);
    char buffer;
    char lineBuffer[LINE_MAX_SIZE];
    int isRead = 1;
    int isCharToFindHere = 0;       // Flaga informująca, czy w linii znajduje się szukany znak

    while(isRead == 1){
        strcpy(lineBuffer, "");
        isCharToFindHere= 0;

        while((isRead = fread(&buffer, sizeof(char), 1, file)) == 1 && buffer != '\n'){
            strncat(lineBuffer, &buffer, 1);

            if(buffer == charToFind)    isCharToFindHere = 1;
        }

        if(isCharToFindHere == 1)   printf("%s\n", lineBuffer);
    }
}

// SYS VERSION

int openFileWithSysToRead(char *fileName){
    int file = open(fileName, O_RDONLY);
    if(file == -1){
        printf("Cann't open %s\n", fileName);
        exit(1);
    }

    return file;
}

void printWithSysOpen(char *fileName, char charToFind){
    int file = openFileWithLibToRead(fileName);
    char buffer;
    char lineBuffer[LINE_MAX_SIZE];
    int isRead = 1;
    int isCharToFindHere = 0;       // Flaga informująca, czy w linii znajduje się szukany znak

    while(isRead == 1){
        strcpy(lineBuffer, "");
        isCharToFindHere= 0;

        while((isRead = read(file, &buffer, sizeof(char))) == 1 && buffer != '\n'){
            strncat(lineBuffer, &buffer, 1);

            if(buffer == charToFind)    isCharToFindHere = 1;
        }

        if(isCharToFindHere == 1)   printf("%s\n", lineBuffer);
    }
}

// MAIN

double calculateDiffrenceBetweenTimes(clock_t startTime, clock_t endTime){
    return (double) (endTime - startTime) / CLOCKS_PER_SEC;
}

void printTimes(double libTime, double sysTime){
    printf("TIMES:\n LIBRARY: %f\n SYSTEM: %f\n\n", libTime, sysTime);
}

void saveTimesToFile(double libTime, double sysTime){
    FILE *file = fopen("pomiar_zad_2.txt", "a");
    fprintf(file, "TIMES:\n LIBRARY: %f\n SYSTEM: %f\n\n", libTime, sysTime);
    fclose(file);
}


int main(int argc, char *argv[]){
    clock_t startTimeSys, endTimeSys, startTimeLib, endTimeLib;
    char charToFind;
    char fileName[FILE_MAX_NAME_SIZE];

    if(argc != 3 || strlen(argv[1]) != 1){
        printf("Bad arguments\n");
        exit(1);
    }

    startTimeLib = clock();
    printWithLibOpen(argv[2], argv[1][0]);      // argv[2] - nazwa pliku ; argv[1][0] - specjalny znak
    endTimeLib = clock();


    startTimeSys = clock();
    printWithSysOpen(argv[2], argv[1][0]);      // argv[2] - nazwa pliku ; argv[1][0] - specjalny znak
    endTimeSys = clock();

    printTimes(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));
    saveTimesToFile(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));

    return 0;
}