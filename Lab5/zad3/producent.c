#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
    char *fifoPath, *pathToFileToRead, *buffer, *lineBuffer;
    int lineID, n;        // n - amount of chars read in one moment
    FILE *fileToRead, *fifoFile;

    srand(time(NULL));

    if(argc != 5){
        printf("Bad ammount of arguments\n");
        exit(1);
    }

    fifoPath = argv[1];
    lineID = atoi(argv[2]);
    pathToFileToRead = argv[3];
    n = atoi(argv[4]);
    buffer = calloc(n, sizeof(char));
    lineBuffer = calloc(n+20, sizeof(char));

    fifoFile = fopen(fifoPath, "w+");

    if(fifoFile == NULL){
        printf("Producent: Can't open fifo file\n");
        exit(1);
    }

    if((fileToRead = fopen(pathToFileToRead, "r")) == NULL){
        printf("Producent: Can't open file to read\n");
        exit(1);
    }

    while(fread(buffer, sizeof(char), n, fileToRead) != NULL){
        sleep(rand()%1+1);
        sprintf(lineBuffer, "%d %s\n", lineID, buffer);
        fputs(lineBuffer, fifoFile);
    }

    free(buffer);
    free(lineBuffer);
    fclose(fifoFile);
    fclose(fileToRead);
    return 0;
}