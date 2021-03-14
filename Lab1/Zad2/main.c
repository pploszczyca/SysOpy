#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/times.h>
#include <unistd.h> 
#include <time.h>
#include "block_library.h"

#define BILLION  1000000000L;

void free3dArray(struct rowMergedFile ** array, int n_array){
    if(array == NULL){
        return;
    }

    for(int i = 0; i < n_array; i++){
        if(array[i] != NULL){
            for(int j = 0; j <  array[i]->n_rows; j++){
                free(array[i]->rows[j]);
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

int main(int argc, char *argv[]){
    struct rowMergedFile ** mainArray = NULL;
    int n_mainArray = 0;
    clock_t real_time[2];

    struct tms **tms_times = initializeNewTmsTimes();

    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "create_table") == 0){
            n_mainArray = atoi(argv[++i]);
            mainArray = makeMainTable(n_mainArray);
        }
        else if(strcmp(argv[i], "merge_files") == 0){
            real_time[0] = times(tms_times[0]);

            for(int j = 0; j < n_mainArray; j++){ 
                char **tmpFileNames = splitTwoFileNames(argv[++i]);
                
                FILE *firstFile = openFileToRead(tmpFileNames[0]);
                FILE *secondFile = openFileToRead(tmpFileNames[1]);

                addFilesToArray(firstFile, secondFile, mainArray, j);

                fclose(firstFile);
                fclose(secondFile);
                freeCharArray(tmpFileNames, 2);
            }

            real_time[1] = times(tms_times[1]);

            
            printf("%lf\n", calculateDiffrenceBetweenTimes(real_time[0], real_time[1]));    // czas rzeczywisty
            printf("%lf\n", calculateDiffrenceBetweenTimes(tms_times[0]->tms_utime, tms_times[1]->tms_utime));  // czas użytkownika
            printf("%lf\n", calculateDiffrenceBetweenTimes(tms_times[0]->tms_stime, tms_times[1]->tms_stime));  // czas systemowy
        }
        else if(strcmp(argv[i], "remove_block") == 0){
            removeOneBlock(mainArray, atoi(argv[++i]));
        }
        else if(strcmp(argv[i], "remove_row") == 0){
            int block_index = atoi(argv[++i]);
            int row_index = atoi(argv[++i]);
            removeOneLineFromBlock(mainArray, block_index, row_index);
        }
        else{
            return 0;
        }
    }

    // print3DArray(mainArray, n_mainArray);

    freeTmsTimes(tms_times);

    free3dArray(mainArray, n_mainArray);
    return 0;
}