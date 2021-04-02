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

void checkDirectors(char *relativePath, char *fileNameToFind ,int depth){
    if(depth == 0){
        return;
    }

    DIR* currentDir = opendir(relativePath);
    struct dirent* directorStructure;
    struct stat path_stat;
    int parentPid;
    
    parentPid = (int) getpid();

    while( (directorStructure = readdir(currentDir)) != NULL){
        stat(directorStructure->d_name, &path_stat);

        if(S_ISDIR(path_stat.st_mode) && strcmp(directorStructure->d_name, ".") != 0 && strcmp(directorStructure->d_name, "..") != 0){
            fork();
            
            if((int) getpid() != parentPid){
                strcat(relativePath, directorStructure->d_name);
                strcat(relativePath, "/");
                checkDirectors(relativePath, fileNameToFind ,depth-1);
                break;
            }
        }

        if(checkIfTxt(directorStructure->d_name) && checkIfContainsName(directorStructure->d_name, fileNameToFind)){
            printf("%s%s  PID: %d\n", relativePath, directorStructure->d_name, parentPid);
        }
    }

    closedir(currentDir);
}

int main(int argc, int *argv[]){
    char *fileNameToFind;
    int maxDepth;
    char relativePath[MAX_PATH_NAME];

    if(argc != 4){
        printf("Bad arguments\n");
        exit(1);
    }

    printf("PID: %d\n",(int) getpid());

    strcpy(relativePath, argv[1]);
    strcat(relativePath, "/");
    fileNameToFind = argv[2];
    maxDepth = atoi(argv[3]);

    checkDirectors(relativePath, fileNameToFind, maxDepth);

    while(wait(NULL) > 0);
    return 0;
}