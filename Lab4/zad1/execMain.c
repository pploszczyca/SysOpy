#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

void checkIfSignalIsInMask(){
    sigset_t pendingMask;

    sigpending(&pendingMask);

    if(sigismember(&pendingMask, SIGUSR1) == 1){
        printf("SIGUSR1 is in mask of exec program\n");
    } else {
        printf("SIGUSR1 is NOT in mask of exec program\n");
    }
}

int main(int argc, char *argv[]){
    if(strcmp(argv[1], "pending") != 0){
        raise(SIGUSR1);
    } else{
        checkIfSignalIsInMask();
    }
}