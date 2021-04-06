#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

static void handleSignal(int signal_no){
    if(signal_no == SIGUSR1){
        printf("Received SIGUSR1!\n");
    } else {
        printf("Received signal %d\n", signal_no);
    }
}

void maskSignal(){
    sigset_t newmask;
    sigset_t oldmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);

    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        perror("Can't block the signal");
}

int main(int argc, char *argv[]){
    pid_t child_pid;

    if(argc != 2){
        printf("Bad ammount of arguments\n");
        exit(1);
    }

    if(strcmp(argv[1], "ignore") == 0){
        sigignore(SIGUSR1);
    }
    else if (strcmp(argv[1], "handler") == 0){
        if(signal(SIGUSR1, handleSignal) == SIG_ERR){
            printf("Can't catch SIGUSR1!\n");
        }
    }
    else if (strcmp(argv[1], "mask") == 0){
        maskSignal();
    }
    else if (strcmp(argv[1], "pending") == 0){
        maskSignal();
    }

    raise(SIGUSR1);

    child_pid = fork();
    if(child_pid == 0 && strcmp(argv[1], "pending") != 0){
        raise(SIGUSR1);
    } else if (child_pid == 0 && strcmp(argv[1], "pending") == 0){
        sigset_t pendingMask;

        sigpending(&pendingMask);

        if(sigismember(&pendingMask, SIGUSR1) == 1){
            printf("SIGUSR1 is in mask\n");
        } else {
            printf("SIGUSR1 is NOT in mask\n");
        }
    }
    
    // printf("PID: %d\n", (int) getpid());

    // for(;;);

    while(wait(NULL) > 0);
    return 0;
}

