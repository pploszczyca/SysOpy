#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h> 
#include <time.h>
#include "block_library.h"

void free3dArray(struct rowMergedFile ** array, int n_array){
    if(array == NULL){
        return;
    }

    for(int i = 0; i < n_array; i++){
        if(array[i] != NULL){
            if(array[i]->rows != NULL){
                for(int j = 0; j <  array[i]->n_rows; j++){
                    free(array[i]->rows[j]);
                }
            }
            free(array[i]);
        }
    }
    free(array);
}

void freeCharArray(char **array, int n_array){
    for(int i = 0; i < n_array; i++){
        free(array[i]);
    }

    free(array);
}

struct tms ** initializeNewTmsTimes(){
    struct tms **tms_times = (struct tms **)calloc(6, sizeof(struct tms *));
    for(int i = 0; i < 6; i++){
        tms_times[i] = (struct tms *)calloc(1, sizeof(struct tms*));
    }

    return tms_times;
}

void freeTmsTimes(struct tms **tms_times){
    free(tms_times);
}


double calculateDiffrenceBetweenTimes(clock_t startTime, clock_t endTime){
    return (double) (endTime - startTime) / sysconf(_SC_CLK_TCK);
}

void printTimes(char* operationName, clock_t real_time[], struct tms **tms_times){
    printf("%25s", operationName);
    printf("%15lf", calculateDiffrenceBetweenTimes(real_time[0], real_time[1]));    // czas rzeczywisty
    printf("%15lf", calculateDiffrenceBetweenTimes(tms_times[0]->tms_utime, tms_times[1]->tms_utime));  // czas użytkownika
    printf("%15lf\n", calculateDiffrenceBetweenTimes(tms_times[0]->tms_stime, tms_times[1]->tms_stime));  // czas systemowy
}

int main(int argc, char *argv[]){
    struct rowMergedFile ** mainArray = NULL;
    int n_mainArray = 1;
    clock_t real_time[2];
    pid_t child_pid;

    struct tms **tms_times = initializeNewTmsTimes();

    real_time[0] = times(tms_times[0]);

    printf("%25s%15s%15s%15s\n", "OPERATION" ,"REAL_TIME", "USER_TIME", "SYSTEM_TIME");

    for(int i = 1; i < argc; i++){
        char **tmpFileNames = splitTwoFileNames(argv[i]);
        child_pid = fork();

        if(child_pid == 0){
            mainArray = makeMainTable(1);
            FILE *firstFile = openFileToRead(tmpFileNames[0]);
            FILE *secondFile = openFileToRead(tmpFileNames[1]);

            addFilesToArray(firstFile, secondFile, mainArray, 0);

            fclose(firstFile);
            fclose(secondFile);
            freeCharArray(tmpFileNames, 2);

            free3dArray(mainArray, 1);

            break;
        }       
    }

    while(wait(NULL) > 0);
    if(child_pid != 0){
        real_time[1] = times(tms_times[1]);
            
        printTimes("Merging", real_time, tms_times);
    }

    freeTmsTimes(tms_times);
    

    return 0;
}