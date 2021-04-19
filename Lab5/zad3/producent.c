#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#define MAX_LINE_BUFFER_SIZE 1000

int main(int argc, char *argv[]){
    char *fifoPath, *pathToFileToRead, *buffer, lineBuffer[MAX_LINE_BUFFER_SIZE];
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
    buffer = calloc(n+10, sizeof(char));


    fifoFile = fopen(fifoPath, "w+");

    if(fifoFile == NULL){
        printf("Can't open fifo file\n");
        exit(1);
    }

    if((fileToRead = fopen(pathToFileToRead, "r")) == NULL){
        printf("Can't open file to read\n");
        exit(1);
    }

    fprintf(fifoFile, "%d ", n);

    while(fread(buffer, sizeof(char), n, fileToRead) != NULL){
        // sleep(rand()%3+1);
        fwrite(buffer, sizeof(char) ,n, fifoFile);
    }

    free(buffer);
    fclose(fifoFile);
    fclose(fileToRead);
    return 0;
}