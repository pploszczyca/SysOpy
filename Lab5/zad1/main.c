#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

#define NAME_MAX_SIZE 100
#define COMMAND_MAX_SIZE 1000
#define MAX_ARGUMENTS_AMMOUNT 100

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
        // free(componentsArray[i]->name);
        // free(componentsArray[i]->command);
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

void clearArray(char commandArguments[MAX_ARGUMENTS_AMMOUNT][NAME_MAX_SIZE], int n){
    for(int i = 0; i < n; i++){
        strcpy(commandArguments[i], "");
    }
}

void runComponents(component **componentsArray, int nOfComponents, FILE *fileWithComponents){
    char streamsBuffer[COMMAND_MAX_SIZE];
    char *nameBuffer;
    char commandName[COMMAND_MAX_SIZE];
    char commandArguments[MAX_ARGUMENTS_AMMOUNT][NAME_MAX_SIZE];
    char *argumentBuffer;
    component *tmpComponent;
    int fd[2];
    int i = 0;
    pid_t childPid;

    while(fscanf(fileWithComponents, "%[^\n]\n", streamsBuffer) != EOF){
        if(fork() == 0){
            pipe(fd);

            nameBuffer = strtok(streamsBuffer, " | ");
            while(nameBuffer != NULL){
                tmpComponent = findComponentInArray(nameBuffer, componentsArray, nOfComponents);
                argumentBuffer = strtok(tmpComponent->command, " ");

                while(argumentBuffer != NULL){
                    clearArray(commandArguments, i);
                    strcpy(commandName, argumentBuffer);
                    // printf("%s\n", commandName);
                    strcpy(commandArguments[0], commandName);
                    
                    argumentBuffer = strtok(NULL, " ");
                    for(i = 1; argumentBuffer != NULL && strcmp(argumentBuffer, "|") != 0; i++){
                        strcpy(commandArguments[i], argumentBuffer);
                        // printf("  %s\n", commandArguments[i]);
                        argumentBuffer = strtok(NULL, " ");
                    }

                    if((childPid = fork()) == 0){
                        close(fd[1]);
                        dup2(fd[0], STDIN_FILENO);

                    } else {
                        close(fd[0]);
                        
                        execvp(commandName, commandArguments);
                        break;
                    }

                    argumentBuffer = strtok(NULL, " ");
                }

                if(childPid != 0){
                    break;
                }

                nameBuffer = strtok(NULL, " | ");
            }

            break;
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