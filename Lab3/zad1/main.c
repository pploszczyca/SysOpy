#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc, int *argv[]) {
    pid_t child_pid;
    int nOfProcess;

    if(argc != 2){
        printf("Niepoprawne argumenty!\n");
        exit(1);
    }

    nOfProcess = atoi(argv[1]);

    for(int i = 0; i < nOfProcess; i++){
        child_pid = fork();
        if(child_pid == 0){
            printf("Ten napis pochodzi z procesu: %d\n", child_pid);
            break;
        }
    }
    
    return 0;
}