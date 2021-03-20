#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LINE_LIMIT 50

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

void brakeLineInFileLib(char *fileToReadName, char *fileToSaveName){
    FILE *fileToRead = openFileWithLibToRead(fileToReadName);
    FILE *fileToSave = openFileWithLibToWrite(fileToSaveName);
    char buffer[1];
    int i = 0;

    while(fread(&buffer, sizeof(char), 1, fileToRead)){
        fwrite(buffer, sizeof(char), 1, fileToSave);
        i++;

        if(i > LINE_LIMIT){
            fwrite("\n", sizeof(char), strlen("\n"), fileToSave);
            i = 0;
        }

        if(buffer[0] == '\n')  i = 0;
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

void brakeLineInFileSys(char *fileToReadName, char *fileToSaveName){
    int fileToRead = openFileWithSysToRead(fileToReadName);
    int fileToSave = openFileWithSysToWrite(fileToSaveName);
    char buffer[1];
    int i = 0;

    while(read(fileToRead, &buffer, sizeof(char))){
        write(fileToSave, buffer, strlen(buffer));
        i++;

        if(i > LINE_LIMIT){
            write(fileToSave, "\n",1);
            i = 0;
        }

        if(buffer[0] == '\n')  i = 0;
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
    FILE *file = fopen("pomiar_zad_5.txt", "a");
    fprintf(file, "TIMES:\n LIBRARY: %f\n SYSTEM: %f\n\n", libTime, sysTime);
    fclose(file);
}

int main(int argc, char *argv[]){
    clock_t startTimeSys, endTimeSys, startTimeLib, endTimeLib;

    
    startTimeLib = clock();
    //Lib Code
    brakeLineInFileLib(argv[1], argv[2]);

    endTimeLib = clock();

    startTimeSys = clock();
    //Sys code
    brakeLineInFileSys(argv[1], argv[2]);
    
    endTimeSys = clock();

    printTimes(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));
    saveTimesToFile(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));

    return 0;
}