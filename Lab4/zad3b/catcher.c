#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

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
    value.sival_int = countSignals;

    for(int i = 0; i < nOfSignals; i++){
        sigqueue(catcherPID, SIGUSR1, value);
    }

    sigqueue(catcherPID, SIGUSR2, value);
}

void sendRealSignalWithKill(int catcherPID, int nOfSignals){
    sendWithKill(catcherPID, nOfSignals, SIGRTMIN+10, SIGRTMIN+15);
}

void handleSignal(int sig_no, siginfo_t *sig_info, void *ucontext){
    countSignals++;
    usleep(250);
    kill((int) sig_info->si_pid, SIGUSR1);
}

void endSignalHandler(int sig_no, siginfo_t *sig_info, void *ucontext){
    HANDLE_SIGNALS = 0;

    if(strcmp(signalSenderType, "KILL") == 0){
        sendWithKill((int)sig_info->si_pid, countSignals, SIGUSR1, SIGUSR2);
    } else if(strcmp(signalSenderType, "SIGQUEUE") == 0){
        sendWithSigqueue((int)sig_info->si_pid, countSignals);
    } else if(strcmp(signalSenderType, "SIGRT") == 0){
        sendRealSignalWithKill((int)sig_info->si_pid, countSignals);
    }
}

int main(int argc, char *argv[]){
    sigset_t mask;
    struct sigaction action;
    struct sigaction endAction;
    signalSenderType = argv[1];
    int startSignal, endSignal;

    if(strcmp(signalSenderType, "SIGRT") == 0){
        startSignal = SIGRTMIN+10;
        endSignal = SIGRTMIN+15;
    } else {
        startSignal = SIGUSR1;
        endSignal = SIGUSR2;
    }

    action.sa_sigaction = handleSignal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    sigaction(startSignal, &action, NULL);

    endAction.sa_sigaction = endSignalHandler;
    sigemptyset(&endAction.sa_mask);
    endAction.sa_flags = SA_SIGINFO;
    sigaction(endSignal, &endAction, NULL);


    sigfillset(&mask);
    sigdelset(&mask, startSignal);
    sigdelset(&mask, endSignal);

    printf("%d\n", (int) getpid());

    while(HANDLE_SIGNALS == 1){
        sigsuspend(&mask);
    }

    printf("Number of received signals: %d\n", countSignals);
    
    return 0;
}