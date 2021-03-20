#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_BUFFER_SIZE 1000

// LIB VERSION

FILE *openFileWithLib(char *fileName, char *options){
    FILE *file = fopen(fileName, options);
    if(file == NULL){
        printf("Cann't open %s\n", fileName);
        exit(1);
    }

    return file;
}

FILE *openFileWithLibToRead(char *fileName){
    return openFileWithLib(fileName, "r");
}

FILE *openFileWithLibToWrite(char *fileName){
    return openFileWithLib(fileName, "w");
}

void changeFunctionLib(char *fileToReadName, char *fileToSaveName, char *n1, char *n2){
    FILE *fileToRead = openFileWithLibToRead(fileToReadName);
    FILE *fileToSave = openFileWithLibToWrite(fileToSaveName);
    char bufferOneChar;
    char buffer[MAX_BUFFER_SIZE];
    int index_n1 = 0;
    int size_n1 = strlen(n1);

    strcpy(buffer, "");

    while(fread(&bufferOneChar, sizeof(char), 1, fileToRead)){
        strncat(buffer, &bufferOneChar, 1);
        if(bufferOneChar == n1[index_n1]){
            index_n1++;
            if(index_n1 == size_n1){
                fwrite(n2, sizeof(char), strlen(n2), fileToSave);
                index_n1 = 0;
                strcpy(buffer, "");
            }
        } else {
            index_n1 = 0;
            fwrite(buffer, sizeof(char), strlen(buffer), fileToSave);
            strcpy(buffer, "");
        }
    }

    fclose(fileToRead);
    fclose(fileToSave);
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

int openFileWithSysToWrite(char *fileName){
    return open(fileName, O_WRONLY | S_IRUSR | O_CREAT, 0644);
}

void changeFunctionSys(char *fileToReadName, char *fileToSaveName, char *n1, char *n2){
    int fileToRead = openFileWithSysToRead(fileToReadName);
    int fileToSave = openFileWithSysToWrite(fileToSaveName);
    char bufferOneChar;
    char buffer[MAX_BUFFER_SIZE];
    int index_n1 = 0;
    int size_n1 = strlen(n1);

    strcpy(buffer, "");

    while(read(fileToRead, &bufferOneChar, sizeof(char))){
        strncat(buffer, &bufferOneChar, 1);
        if(bufferOneChar == n1[index_n1]){
            index_n1++;
            if(index_n1 == size_n1){
                write(fileToSave, n2, strlen(n2));
                index_n1 = 0;
                strcpy(buffer, "");
            }
        } else {
            index_n1 = 0;
            write(fileToSave, buffer, strlen(buffer));
            strcpy(buffer, "");
        }
    }

    close(fileToRead);
    close(fileToSave);
}


// MAIN

double calculateDiffrenceBetweenTimes(clock_t startTime, clock_t endTime){
    return (double) (endTime - startTime) / CLOCKS_PER_SEC;
}

void printTimes(double libTime, double sysTime){
    printf("TIMES:\n LIBRARY: %f\n SYSTEM: %f\n\n", libTime, sysTime);
}

void saveTimesToFile(double libTime, double sysTime){
    FILE *file = fopen("pomiar_zad_4.txt", "a");
    fprintf(file, "TIMES:\n LIBRARY: %f\n SYSTEM: %f\n\n", libTime, sysTime);
    fclose(file);
}

int main(int argc, char *argv[]){
    clock_t startTimeSys, endTimeSys, startTimeLib, endTimeLib;

    
    startTimeLib = clock();
    //Lib Code
    changeFunctionLib(argv[1], argv[2], argv[3], argv[4]);

    endTimeLib = clock();

    startTimeSys = clock();
    //Sys code
    changeFunctionSys(argv[1], argv[2], argv[3], argv[4]);
    
    endTimeSys = clock();

    printTimes(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));
    saveTimesToFile(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));

    return 0;
}