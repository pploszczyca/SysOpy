#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    char *fifoPath, *pathToFileToWrite, *buffer;
    int n;        // n - amount of chars read in one moment
    FILE *fifoFile, *fileToWrite;

    if(argc != 4){
        printf("Bad ammount of arguments\n");
        exit(1);
    }

    fifoPath = argv[1];
    pathToFileToWrite = argv[2];
    n = atoi(argv[3]);
    buffer = calloc(n, sizeof(char));

    if((fifoFile = fopen(fifoPath, "r+")) == NULL){
        printf("Can't open fifo file\n");
        exit(1);
    }

    if((fileToWrite = fopen(pathToFileToWrite, "w")) == NULL){
        printf("Can't open file to read\n");
        exit(1);
    }

    fread(buffer, sizeof(char) ,n, fifoFile);
    fwrite(buffer,sizeof(char), n ,fileToWrite);

    free(buffer);
    fclose(fifoFile);
    fclose(fileToWrite);
    return 0;
}