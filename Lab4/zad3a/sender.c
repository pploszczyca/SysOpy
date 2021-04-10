#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

int countSignals = 0;
int HANDLE_SIGNALS = 1;
char *signalSenderType;

void sendWithKill(int catcherPID, int nOfSignals, int startSignal, int endSignal){
    for(int i = 0; i < nOfSignals; i++){
        kill(catcherPID, startSignal);
    }
    kill(catcherPID, endSignal);
}

void sendWithSigqueue(int catcherPID, int nOfSignals){
    union sigval value;

    for(int i = 0; i < nOfSignals; i++){
        sigqueue(catcherPID, SIGUSR1, value);
    }

    sigqueue(catcherPID, SIGUSR2, value);
}

void sendRealSignalWithKill(int catcherPID, int nOfSignals){
    sendWithKill(catcherPID, nOfSignals, SIGRTMIN+10, SIGRTMIN+15);
}

void handleSignal(int sig_no){
    countSignals++;
}

void endSignalHandler(int sig_no, siginfo_t *sig_info, void *ucontext){
    HANDLE_SIGNALS = 0;
    printf("Number of received signals: %d\n", countSignals);

    if(strcmp(signalSenderType, "SIGQUEUE") == 0){
        printf("Number of signals that catcher received: %d\n", sig_info->si_value.sival_int);
    }
}

int main(int argc, char *argv[]){
    int catcherPID;
    int nOfSignals;
    sigset_t mask;
    struct sigaction endAction;
    signalSenderType = argv[3];

    int startSignal, endSignal;

    if(strcmp(signalSenderType, "SIGRT") == 0){
        startSignal = SIGRTMIN+10;
        endSignal = SIGRTMIN+15;
    } else {
        startSignal = SIGUSR1;
        endSignal = SIGUSR2;
    }

    signal(startSignal, handleSignal);

    endAction.sa_sigaction = endSignalHandler;
    sigemptyset(&endAction.sa_mask);
    endAction.sa_flags = SA_SIGINFO;
    sigaction(endSignal, &endAction, NULL);


    sigfillset(&mask);
    sigdelset(&mask, startSignal);
    sigdelset(&mask, endSignal);

    if(argc != 4){
        printf("Bad ammount of arguments\n");
        exit(1);
    }

    catcherPID = atoi(argv[1]);
    nOfSignals = atoi(argv[2]);

    if(strcmp(argv[3], "KILL") == 0){
        sendWithKill(catcherPID, nOfSignals, SIGUSR1, SIGUSR2);
    } else if(strcmp(argv[3], "SIGQUEUE") == 0){
        sendWithSigqueue(catcherPID, nOfSignals);
    } else if(strcmp(argv[3], "SIGRT") == 0){
        sendRealSignalWithKill(catcherPID, nOfSignals);
    }

    while(HANDLE_SIGNALS == 1){
        sigsuspend(&mask);
    }

    return 0;
}