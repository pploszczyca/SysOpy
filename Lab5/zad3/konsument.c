#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LINE_AMOUNT 20
#define MAX_LINE_BUFFER 1000

int main(int argc, char *argv[]){
    char *fifoPath, *pathToFileToWrite;
    char lineBuffers[MAX_LINE_AMOUNT][MAX_LINE_BUFFER], lineTmp[10];
    int n, maxReadLine = 0;        // n - amount of chars read in one moment
    FILE *fifoFile, *fileToWrite;

    if(argc != 4){
        printf("Bad ammount of arguments\n");
        exit(1);
    }

    fifoPath = argv[1];
    pathToFileToWrite = argv[2];
    n = atoi(argv[3]);
    char buffer[n], lineBuffer[n];

    if((fifoFile = fopen(fifoPath, "r")) == NULL){
        printf("Konsument: Can't open fifo file\n");
        exit(1);
    }

    if((fileToWrite = fopen(pathToFileToWrite, "w")) == NULL){
        printf("Konsument: Can't open file to read\n");
        exit(1);
    }

    for(int i = 0; i < MAX_LINE_AMOUNT; i++){
        strcpy(lineBuffers[i], "");
    }

    while(fgets(buffer, n, fifoFile) != NULL){
        sscanf(buffer, "%s %1000[^\n]\n", lineTmp, lineBuffer);

        printf("%s\n", lineBuffer);
        
        strcat(lineBuffers[atoi(lineTmp)-1], lineBuffer);

        if(atoi(lineTmp) > maxReadLine){
            maxReadLine = atoi(lineTmp);
        }
    }

    for(int i = 0; i < maxReadLine; i++){
        if(strcmp(lineBuffers[i], "") != 0){
            fprintf(fileToWrite, "%d %s\n", i+1, lineBuffers[i]);
        } else {
            fprintf(fileToWrite, "\n");
        }
    }

    fclose(fifoFile);
    fclose(fileToWrite);
    return 0;
}