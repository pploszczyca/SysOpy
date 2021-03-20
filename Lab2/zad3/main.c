#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> 
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

#define FILE_TO_READ_NAME "dane.txt"
#define A_FILE_NAME "a.txt"
#define B_FILE_NAME "b.txt"
#define C_FILE_NAME "c.txt"
#define NUMBER_MAX_SIZE 50

int checkEven(int number){
    return number%2 == 0;
}

int checkIfIsSevenOrZero(int number){
    return !(number < 10) && ( ((number - (number%10))/10)%10 == 0 || ((number - (number%10))/10)%10 == 7 );
}

int checkIfRootIsTotal(int number){
    return floor(sqrt(number)) == sqrt(number);
}

// LIB VERSION

FILE *openFileWithLib(char *fileName, char *options){
    FILE *file = fopen(fileName, options);
    if(file == NULL){
        printf("Cann't open %s\n", fileName);
        exit(1);
    }

    return file;
}

FILE *openFileWithLibToRead(char *fileName){
    return openFileWithLib(fileName, "r");
}

FILE *openFileWithLibToWrite(char *fileName){
    return openFileWithLib(fileName, "w");
}

void saveStringToFileLib(FILE *file, char *result){
    fwrite(result, sizeof(char), strlen(result), file);
}

void saveNumberToFileLib(FILE *file, int number){
    char stringNumber[NUMBER_MAX_SIZE];
    sprintf(stringNumber, "%d ", number);
    saveStringToFileLib(file, stringNumber);
}


int readOneNumberFromFileLib(FILE *file){      // if there is end of file then will return -1
    char buffer;
    char numberBuffer[NUMBER_MAX_SIZE];
    int isRead;

    strcpy(numberBuffer, "");

    while((isRead = fread(&buffer, sizeof(char), 1, file)) == 1 && buffer != '\n' && buffer != ' '){
        strncat(numberBuffer, &buffer, 1);
    }

    return (isRead != 1 && strlen(numberBuffer) == 0)? -1 : atoi(numberBuffer);
}

void findNumbersLib(){
    FILE *file = openFileWithLibToRead(FILE_TO_READ_NAME);
    FILE *aFile = openFileWithLibToWrite(A_FILE_NAME);
    FILE *bFile = openFileWithLibToWrite(B_FILE_NAME);
    FILE *cFile = openFileWithLibToWrite(C_FILE_NAME);
    int evenNumbersAmmount = 0;
    int number;
    char a_result[NUMBER_MAX_SIZE];
    
    while((number = readOneNumberFromFileLib(file)) != -1){
        if(checkEven(number) == 1)  evenNumbersAmmount++;
        if(checkIfIsSevenOrZero(number) == 1)   saveNumberToFileLib(bFile, number);
        if(checkIfRootIsTotal(number) == 1) saveNumberToFileLib(cFile, number);
    }

    sprintf(a_result, "Liczb parzystych jest %d", evenNumbersAmmount);
    saveStringToFileLib(aFile, a_result);

    fclose(aFile);
    fclose(bFile);
    fclose(cFile);
    fclose(file);
}


//SYS VERSION

int openFileWithSysToRead(char *fileName){
    int file = open(fileName, O_RDONLY);
    if(file == -1){
        printf("Cann't open %s\n", fileName);
        exit(1);
    }

    return file;
}

int openFileWithSysToWrite(char *fileName){
    return open(fileName, O_WRONLY | S_IRUSR | O_CREAT, 0644);
}

void saveStringToFileSys(int file, char *result){
    write(file, result, strlen(result));
}

void saveNumberToFileSys(int file, int number){
    char stringNumber[NUMBER_MAX_SIZE];
    sprintf(stringNumber, "%d ", number);
    saveStringToFileSys(file, stringNumber);
}

int readOneNumberFromFileSys(int file){      // if there is end of file then will return -1
    char buffer;
    char numberBuffer[NUMBER_MAX_SIZE];
    int isRead;

    strcpy(numberBuffer, "");

    while((isRead = read(file, &buffer, sizeof(char))) == 1 && buffer != '\n' && buffer != ' '){
        strncat(numberBuffer, &buffer, 1);
    }

    return (isRead != 1 && strlen(numberBuffer) == 0)? -1 : atoi(numberBuffer);
}

void findNumbersSys(){
    int file = openFileWithSysToRead(FILE_TO_READ_NAME);
    int aFile = openFileWithSysToWrite(A_FILE_NAME);
    int bFile = openFileWithSysToWrite(B_FILE_NAME);
    int cFile = openFileWithSysToWrite(C_FILE_NAME);
    int evenNumbersAmmount = 0;
    int number;
    char a_result[NUMBER_MAX_SIZE];
    
    while((number = readOneNumberFromFileSys(file)) != -1){
        if(checkEven(number) == 1)  evenNumbersAmmount++;
        if(checkIfIsSevenOrZero(number) == 1)   saveNumberToFileSys(bFile, number);
        if(checkIfRootIsTotal(number) == 1) saveNumberToFileSys(cFile, number);
    }

    sprintf(a_result, "Liczb parzystych jest %d", evenNumbersAmmount);
    saveStringToFileSys(aFile, a_result);

    close(aFile);
    close(bFile);
    close(cFile);
    close(file);
}


// MAIN

double calculateDiffrenceBetweenTimes(clock_t startTime, clock_t endTime){
    return (double) (endTime - startTime) / CLOCKS_PER_SEC;
}

void printTimes(double libTime, double sysTime){
    printf("TIMES:\n LIBRARY: %f\n SYSTEM: %f\n\n", libTime, sysTime);
}

void saveTimesToFile(double libTime, double sysTime){
    FILE *file = fopen("pomiar_zad_3.txt", "a");
    fprintf(file, "TIMES:\n LIBRARY: %f\n SYSTEM: %f\n\n", libTime, sysTime);
    fclose(file);
}


int main(int argc, char *argv[]){
    clock_t startTimeSys, endTimeSys, startTimeLib, endTimeLib;
    
    startTimeLib = clock();
    findNumbersLib();
    endTimeLib = clock();

    startTimeSys = clock();
    findNumbersSys();
    endTimeSys = clock();

    printTimes(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));
    saveTimesToFile(calculateDiffrenceBetweenTimes(startTimeLib, endTimeLib), calculateDiffrenceBetweenTimes(startTimeSys, endTimeSys));

    return 0;
}