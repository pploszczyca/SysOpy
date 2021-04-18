#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

#define NAME_MAX_SIZE 100
#define COMMAND_MAX_SIZE 1000

typedef struct component {
    char name[NAME_MAX_SIZE];
    char command[COMMAND_MAX_SIZE];
} component;

component ** initializeComponentsArray(int nOfComponents){
    component **componentsArray = (component **)calloc(nOfComponents, sizeof(component *));
    for(int i = 0; i < nOfComponents; i++){
        componentsArray[i] = (component *)calloc(1, sizeof(component));
    }

    return componentsArray;
}

void freeComponentsArray(component **componentsArray, int nOfComponents){
    for(int i = 0; i < nOfComponents; i++){
        free(componentsArray[i]);
    }
}

void saveComponentsFromFileToArray(component **componentsArray, int nOfComponents, FILE *fileWithComponents){
    for(int i = 0; i < nOfComponents; i++){
        fscanf(fileWithComponents, "%s = %1000[^\n]\n" ,componentsArray[i]->name, componentsArray[i]->command);
    }
}

void printComponentsArray(component **componentsArray, int nOfComponents){
    for(int i = 0; i < nOfComponents; i++){
        printf("%s = %s\n", componentsArray[i]->name, componentsArray[i]->command);
    }
}

component * findComponentInArray(char *nameToFind, component **componentsArray, int nOfComponents){
    for(int i = 0; i < nOfComponents; i++){
        if(strcmp(componentsArray[i]->name, nameToFind) == 0){
            return componentsArray[i];
        }
    }

    return NULL;
}

void runComponents(component **componentsArray, int nOfComponents, FILE *fileWithComponents){
    char streamsBuffer[COMMAND_MAX_SIZE];
    char bufferOfResult[COMMAND_MAX_SIZE];
    char *nameBuffer;
    component *tmpComponent;
    int fd[2], fdPrevious[2] = {-1, -1};
    int i = 0;
    pid_t childPid;

    while(fscanf(fileWithComponents, "%[^\n]\n", streamsBuffer) != EOF){
        strcpy(bufferOfResult, "");

        nameBuffer = strtok(streamsBuffer, " | ");
        while(nameBuffer != NULL){
            pipe(fd);
            tmpComponent = findComponentInArray(nameBuffer, componentsArray, nOfComponents);

            nameBuffer = strtok(NULL, " | ");

            childPid = fork();

            if(childPid == 0){
                close(fdPrevious[1]);
                dup2(fdPrevious[0], STDIN_FILENO);

                if(nameBuffer != NULL){
                    close(fd[0]);
                    dup2(fd[1], STDOUT_FILENO);
                }
                
                execl("/bin/sh", "/bin/sh", "-c", tmpComponent->command, (char *) NULL);
            } 
                
            close(fd[1]);
            fdPrevious[0] = fd[0];
            fdPrevious[1] = fd[1];
        }

        while(wait(NULL) > 0);
    }
}

int main(int argc, char *argv[]){
    component **componentsArray;
    char *filePath;
    int nOfComponents;
    FILE *fileWithComponents;

    if(argc != 3){
        printf("Bad ammount of arguments\n");
        exit(1);
    }

    filePath = argv[1];
    nOfComponents = atoi(argv[2]);

    if((fileWithComponents = fopen(filePath, "r")) == NULL){
        printf("Error! File cannot be opened.");
        exit(1);
    }

    componentsArray = initializeComponentsArray(nOfComponents);

    saveComponentsFromFileToArray(componentsArray, nOfComponents, fileWithComponents);

    // printComponentsArray(componentsArray, nOfComponents);
    runComponents(componentsArray, nOfComponents, fileWithComponents);

    fclose(fileWithComponents);
    freeComponentsArray(componentsArray, nOfComponents);

    while(wait(NULL) > 0);
    return 0;
}