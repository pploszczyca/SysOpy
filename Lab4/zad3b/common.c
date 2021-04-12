#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <stdlib.h>

int countSignals = 0;
int HANDLE_SIGNALS = 1;
int nSignalsToSend = 0;

void handleSignal(int sig_no, siginfo_t *sig_info, void *ucontext);
void endSignalHandler(int sig_no, siginfo_t *sig_info, void *ucontext);

void setMasksAndHandlers(int startSignal, int endSignal, sigset_t *mask){
    struct sigaction action;
    struct sigaction endAction;

    action.sa_sigaction = handleSignal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_SIGINFO;
    sigaction(startSignal, &action, NULL);

    endAction.sa_sigaction = endSignalHandler;
    sigemptyset(&endAction.sa_mask);
    endAction.sa_flags = SA_SIGINFO;
    sigaction(endSignal, &endAction, NULL);

    sigfillset(mask);
    sigdelset(mask, startSignal);
    sigdelset(mask, endSignal);
}