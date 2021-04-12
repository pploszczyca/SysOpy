#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

int countSignals = 0;
int HANDLE_SIGNALS = 1;
char *signalSenderType;

void endSignalHandler(int sig_no, siginfo_t *sig_info, void *ucontext);

void sendWithKill(int catcherPID, int nOfSignals, int startSignal, int endSignal){
    for(int i = 0; i < nOfSignals; i++){
        kill(catcherPID, startSignal);
    }
    kill(catcherPID, endSignal);
}

void sendWithSigqueue(int catcherPID, int nOfSignals){
    union sigval value;
    value.sival_int = countSignals;

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

void setStartEndSignal(int *startSignal, int *endSignal){
    if(strcmp(signalSenderType, "SIGRT") == 0){
        *startSignal = SIGRTMIN+10;
        *endSignal = SIGRTMIN+15;
    } else {    
        *startSignal = SIGUSR1;
        *endSignal = SIGUSR2;
    }
}

void chooseSignalMethod(int catcherPID, int nOfSignals){
    if(strcmp(signalSenderType, "KILL") == 0){
        sendWithKill(catcherPID, nOfSignals, SIGUSR1, SIGUSR2);
    } else if(strcmp(signalSenderType, "SIGQUEUE") == 0){
        sendWithSigqueue(catcherPID, nOfSignals);
    } else if(strcmp(signalSenderType, "SIGRT") == 0){
        sendRealSignalWithKill(catcherPID, nOfSignals);
    }
}

void setMasksAndHandlers(int startSignal, int endSignal, sigset_t *mask){
    struct sigaction endAction;

    signal(startSignal, handleSignal);

    endAction.sa_sigaction = endSignalHandler;
    sigemptyset(&endAction.sa_mask);
    endAction.sa_flags = SA_SIGINFO;
    sigaction(endSignal, &endAction, NULL);

    sigfillset(mask);
    sigdelset(mask, startSignal);
    sigdelset(mask, endSignal);
}