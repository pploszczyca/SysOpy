#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MAX_PATH_NAME 1000

int checkIfTxt(char *fileName){
    int nFileName = strlen(fileName);

    if(nFileName < 5){
        return 0;
    }

    return fileName[nFileName-1] == 't' && fileName[nFileName-2] == 'x' && fileName[nFileName-3] == 't' && fileName[nFileName-4] == '.';
}

int checkIfContainsName(char *fileName, char* fileNameToFind){
    int nFileName = strlen(fileName);
    int nFileNameToFind = strlen(fileNameToFind);

    if(nFileName < nFileNameToFind) return 0;
    
    for(int i = 0, j=0; i < nFileName; i++){
        if(fileName[i] == fileNameToFind[j]){
            j++;
            if(j == nFileNameToFind){
                return 1;
            }
        } else {
            j = 0;
        }
    }

    return 0;
}

void checkDirectors(char *directorName, char *fileNameToFind ,int depth, char *relativePath){
    if(depth == 0){
        return;
    }

    DIR* currentDir = opendir(directorName);
    struct dirent* directorStructure;
    struct stat path_stat;
    int parentPid;

    strcat(relativePath, directorName);
    strcat(relativePath, "/");

    // printf("PID: %d - %s\n",(int) getpid(),relativePath);

    while( (directorStructure = readdir(currentDir)) != NULL){
        stat(directorStructure->d_name, &path_stat);

        // printf("%s\n", directorStructure->d_name);

        parentPid = (int) getpid();

        if(S_ISDIR(path_stat.st_mode)==0 && strcmp(directorStructure->d_name, ".") != 0 && strcmp(directorStructure->d_name, "..") != 0){
            fork();
            printf("%s\n", directorStructure->d_name);
            if((int) getpid() != parentPid){
                checkDirectors(directorStructure->d_name, fileNameToFind ,depth-1, relativePath);
                break;
            }
        }

        if(checkIfTxt(directorStructure->d_name) && checkIfContainsName(directorStructure->d_name, fileNameToFind)){
            printf("%s%s  PID: %d\n", relativePath, directorStructure->d_name, (int) getpid());
        }
    }

    closedir(currentDir);
}

int main(int argc, int *argv[]){
    char *startCatalog, *fileNameToFind;
    int maxDepth;
    char relativePath[MAX_PATH_NAME];

    strcpy(relativePath, "");

    if(argc != 4){
        printf("Bad arguments\n");
        exit(1);
    }

    printf("PID: %d\n",(int) getpid());

    startCatalog = argv[1];
    fileNameToFind = argv[2];
    maxDepth = atoi(argv[3]);

    checkDirectors(startCatalog, fileNameToFind, maxDepth, relativePath);

    while(wait(NULL) > 0);
    return 0;
}