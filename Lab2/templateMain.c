#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

double calculateDiffrenceBetweenTimes(clock_t startTime, clock_t endTime){
    return (double) (endTime - startTime) / CLOCKS_PER_SEC;
}

void printTimes(double libTime, double sysTime){
    printf("TIMES:\n LIBRARY: %f\n SYSTEM: %f\n\n", libTime, sysTime);
}

void saveTimesToFile(double libTime, double sysTime){
    FILE *file = fopen("pomiar_zad_1.txt", "a");
    fprintf(file, "TIMES:\n LIBRARY: %f\n SYSTEM: %f\n\n", libTime, sysTime);
    fclose(file);
}


int main(int argc, char *argv[]){
    clock_t startTimeSys, endTimeSys, startTimeLib, endTimeLib;

    
    startTimeLib = clock();
    //Lib Code


    endTimeLib = clock();

    startTimeSys = clock();
    //Sys code
    
    
    endTimeSys = clock();

    printTimes(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));
    saveTimesToFile(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));

    return 0;
}